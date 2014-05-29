#include "AtlasRegistration.h"

AtlasRegistration::AtlasRegistration(QString module) : Script(module)
{

}

// SET // 
void AtlasRegistration::setLog(QString log_path)
{
   m_log_path = log_path; 
}
void AtlasRegistration::setAntsParameters(AntsParameters* parameters) 
{
   m_parameters=parameters;
}

void AtlasRegistration::initializeScript()
{
   definePython();
   importGeneralModules();

   defineExecutable("ANTS");
   defineExecutable("ResampleVolume2");
   defineExecutable("ImageMath");
   defineExecutable("ImageStat");
   defineExecutable("unu");
   defineExecutable("CleanRingMask");
   m_script += "import re\n\n";
   m_script += "import csv\n\n";
 
   m_script += "logger = None\n";

   m_script += "runningProcess = None\n";  
   m_script += "logFile = None\n\n";  
}

void AtlasRegistration::initializeLogging()
{
   m_script += "def initializeLogging(log):\n";

   m_script += "\tglobal logger\n";   
   m_script += "\tlogger = logging.getLogger('NeosegPipelineRegistrations')\n"; 
   m_script += "\tlogger.setLevel(logging.DEBUG)\n";

   m_script += "\tfileHandler = logging.FileHandler(log)\n";
   m_script += "\tfileHandler.setLevel(logging.DEBUG)\n";
   m_script += "\tfileFormatter = logging.Formatter('%(message)s')\n";
   m_script += "\tfileHandler.setFormatter(fileFormatter)\n";

   m_script += "\tlogger.addHandler(fileHandler)\n\n";
}

void AtlasRegistration::implementRegisterAtlas(bool probabilistic)
{   
   if(probabilistic)
   {
      m_script += "def main(name, T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, output, log):\n\n";
   } 
   else
   {
      m_script += "def main(name, T1Atlas, T2Atlas, segAtlas, output, log):\n\n";
   }

   m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
   m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   m_script += "\tlogFile = open(log, \"w\")\n";
   m_script += "\tinitializeLogging(log)\n\n";

   m_script += "\toutbase = output + '/' + name + '_to_" + m_neo.prefix + "'\n";   
   m_script += "\tfinalT1 = outbase + '-T1.nrrd'\n";
   m_script += "\tfinalT2 = outbase + '-T2.nrrd'\n";

   if(probabilistic)
   {
      m_script += "\tfinalWhite = outbase + '-white.nrrd'\n";
      m_script += "\tfinalGray = outbase + '-gray.nrrd'\n";
      m_script += "\tfinalCsf = outbase + '-csf.nrrd'\n";
      m_script += "\tif checkFileExistence(finalT1)==True and checkFileExistence(finalT2) and checkFileExistence(finalWhite) and checkFileExistence(finalGray) and checkFileExistence(finalCsf) :\n";
   }
   else
   {
      m_script += "\tfinalSeg = outbase + '-csf.nrrd'\n";
      m_script += "\tif checkFileExistence(finalT1)==True and checkFileExistence(finalT2) and checkFileExistence(finalSeg) :\n";
   }

   m_script += "\t\tmainLog = open('" + m_log_path + "','w')\n"; 
   m_script += "\t\tmainLog.write(name + ' Registration -> Skipped\\n')\n"; 
   m_script += "\t\tmainLog.close()\n";  
   m_script += "\t\treturn\n"; 

   m_script += "\tmainLog = open('" + m_log_path + "','a')\n"; 
   m_script += "\tmainLog.write(name + ' Registration...\\n')\n"; 
   m_script += "\tmainLog.close()\n";  

   m_script += "\tT1 = '" + m_neo.T1 + "'\n";
   m_script += "\tT2 = '" + m_neo.T2 + "'\n";  
   m_script += "\tmask = '" + m_neo.mask + "'\n\n";  

   if(m_parameters->getAddingExtraCSF())
   {
      // Create Mask Based on the segmentation
      /*QString maskSegmentation_path = "' + output + '/" + m_prefix + "mask-segmentation" + m_suffix + ".nrrd";

      m_log = "Create mask";
      m_outputs.insert("maskSegmentation", maskSegmentation_path); 
      m_argumentsList << "ImageMath" << "segAtlas" << "'-threshold'" << "'1,4'" << "'-outfile'" << "maskSegmentation";
      execute();*/ 

      // Create Mask Based on the image
      QString maskImage_path = "' + output + '/' + name + 'mask-image" + m_suffix + ".nrrd";

      m_log = "Create mask";
      m_outputs.insert("mask", maskImage_path); 
      m_argumentsList << "ImageMath" << "T2Atlas" << "'-threshold'" << "'1,1000000'" << "'-outfile'" << "mask";
      execute();   

      // Itersection of the two masks 
      /*QString mask_path = "' + output + '/" + m_prefix + "mask" + m_suffix + ".nrrd";

      m_log = "Create mask";
      m_outputs.insert("mask", mask_path); 
      m_argumentsList << "ImageMath" << "maskImage" << "'-mask'" << "maskSegmentation" << "'-outfile'" << "mask";
      execute();*/

      // Dilate Mask
      QString dilatedMask_path = "' + output + '/' + name + 'mask-dilated" + m_suffix + ".nrrd";

      m_log = "Dilate mask";
      m_outputs.insert("dilatedMask", dilatedMask_path); 
      m_argumentsList << "ImageMath" << "mask" << "'-dilate'" << "'3,1'" << "'-outfile'" << "dilatedMask";
      execute(); 

      // Sum Mask
      QString sumMask_path = "' + output + '/' + name + 'mask-sum" + m_suffix + ".nrrd";

      m_log = "Sum both masks";   
      m_outputs.insert("sumMask", sumMask_path); 
      m_argumentsList << "ImageMath" << "mask" << "'-add'" << "dilatedMask" << "'-outfile'" << "sumMask";
      execute(); 

      // Ring Mask
      QString ringMask_path = "' + output + '/' + name + 'mask-ring" + m_suffix + ".nrrd";

      m_log = "Computing ring mask";   
      m_outputs.insert("ringMask", ringMask_path); 
      m_argumentsList << "ImageMath" << "sumMask" << "'-extractLabel'" << "'1'" << "'-outfile'" << "ringMask";
      execute(); 

      // Clean Ring Mask 
      QString cleanedRingMask_path = "' + output + '/' + name + 'mask-ring-cleaned" + m_suffix + ".nrrd";

      m_log = "- Cleaning ring mask";
      m_outputs.insert("cleanedRingMask", cleanedRingMask_path);
      m_argumentsList << "CleanRingMask" << "'--input'" << "ringMask" << "'--output'" << "cleanedRingMask";
      execute(); 

      // CSF intensity
      QString outbase_path = "' + output + '/' + name + 'T2";

      m_log = "Computing CSF intensity on T2"; 
      m_outputs.insert("CSVoutbase", outbase_path);
      m_argumentsList << "ImageStat" << "T2" << "'-label'" << "mask" << "'-intensitySummary'" << "'-outbase'" << "CSVoutbase";
      execute();

      // CSF ring
      QString csv_path = "' + output + '/' + name + 'T2_intensitySummary.csv"; 
      QString oper = "2,' + str([row for row in csv.reader(open(csv_file, 'rb'))][9][2]) + '";
      QString CSFRing_path = "' + output + '/" + m_prefix + "CSF-ring" + m_suffix + ".nrrd";

      m_log = "Multyplying ring mask by CSF intensity value";
      m_inputs.insert("csv_file", csv_path);
      m_inputs.insert("oper", oper);
      m_outputs.insert("CSFRing", CSFRing_path); 
      m_argumentsList << "ImageMath" << "ringMask" << "'-constOper'" << "oper" << "'-type'" << "'float'" << "'-outfile'" << "CSFRing";
      execute(); 

      // T2 with extra CSF
      QString T2WithExtraCSF_path = "' + output + '/' + name + '-T2-extraCSF" + m_suffix + ".nrrd";

      m_log = "Adding CSF ring to T2"; 
      m_outputs.insert("T2Atlas_withExtraCSF", T2WithExtraCSF_path); 
      m_argumentsList << "ImageMath" << "T2Atlas" << "'-add'" << "CSFRing" << "'-type'" << "'float'" << "'-outfile'" << "T2Atlas_withExtraCSF";
      execute();
   }

   // Smooth mask
   if(m_parameters->getUsingSmoothedMask())
   {
      QString smoothedMask_path = "' + output + '/" + m_prefix + "mask-smoothed" + m_suffix + ".nrrd";

      m_log = "Smoothing mask";
      m_inputs.insert("mask", m_neo.mask);   
      m_outputs.insert("smoothedMask", smoothedMask_path); 
      m_argumentsList << "ImageMath" << "mask" << "'-smooth'" << "'-gauss'" << "'-size'" << "'1'" << "'-outfile'" << "smoothedMask";
      execute(); 
   } 

   // Registration
   QString modality1 = "CC[' + T1 + ',' + T1Atlas + '," + QString::number(m_parameters->getWeight1()) + "," + QString::number(m_parameters->getRadius1()) + "]";
   QString modality2; 
   if(m_parameters->getAddingExtraCSF())
   {
      m_inputs.insert("T2_withExtraCSF", m_neo.T2Modified);
      modality2 = "CC[' + T2_withExtraCSF + ',' + T2Atlas_withExtraCSF + '," + QString::number(m_parameters->getWeight2()) + "," + QString::number(m_parameters->getRadius2()) + "]";
   }
   else
   {
      modality2 = "CC[' + T2 + ',' + T2Atlas + '," + QString::number(m_parameters->getWeight2()) + "," + QString::number(m_parameters->getRadius2()) + "]";
   }
   QString iterations = QString::number(m_parameters->getIterationsJ()) + "x" + QString::number(m_parameters->getIterationsK()) + "x" + QString::number(m_parameters->getIterationsL());
   QString transformation = m_parameters->getTransformationType() + "[" + QString::number(m_parameters->getGradientStepLength()) + "]"; //"," + QString::number(m_parameters->getNumberOfTimeSteps()) + "," + QString::number(m_parameters->getDeltaTime()) + "]";
   QString regularization = m_parameters->getRegularizationType() + "[" + QString::number(m_parameters->getGradientFieldSigma()) + "," + QString::number(m_parameters->getDeformationFieldSigma()) + "]";

   QString output = "' + outbase + '_.nii.gz";
   QString affine = "' + outbase + '_Affine.txt";
   QString warp = "' + outbase + '_Warp.nii.gz";

   m_log = "Calculating transformations";

   m_inputs.insert("modality1", modality1); 
   m_inputsTests.insert("modality1", "T1Atlas");
   m_inputs.insert("modality2", modality2); 
   m_inputsTests.insert("modality2", "T2Atlas");
   m_inputs.insert("iterations", iterations); 
   m_inputs.insert("transformation", transformation);
   m_inputs.insert("regularization", regularization);  
   m_inputs.insert("output", output );

   m_outputs.insert("affine", affine);
   m_outputs.insert("warp", warp);  

   QString mask; 
   if(m_parameters->getUsingMask())
   {
      mask = "mask";
      m_inputs.insert("mask", m_neo.mask); 
   }
   else if(m_parameters->getAddingExtraCSF())
   {
      mask = "dilatedMask"; 
      m_inputs.insert("dilatedMask", m_neo.dilatedMask); 
   }
   else if(m_parameters->getUsingSmoothedMask())
   {
      mask = "smoothedMask";
   }

   m_argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization";

   QStringList argsTests_T1; 
   argsTests_T1 << "ANTS" << "'3'" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization";

   QStringList argsTests_T2; 
   argsTests_T2 << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization";

   if(m_parameters->getUsingMask() || m_parameters->getUsingSmoothedMask() || m_parameters->getAddingExtraCSF())
   {
      m_argumentsList << "'-x'" << mask; 
      argsTests_T1 << "'-x'" << mask; 
      argsTests_T2 << "'-x'" << mask; 
   }

   m_argsTests.insert("T1Atlas", argsTests_T1); 
   m_argsTests.insert("T2Atlas", argsTests_T2); 
   execute(); 

   // Normalizing affine file
   m_log = "Normalizing affine file";
   m_argumentsList << "'text_subst.pl'" << "'MatrixOffsetTransformBase_double_3_3'" << "'AffineTransform_double_3_3'" << "affine"; 
   execute();

   // Applying transformations to T1
   QString T1Reg = "' + outbase + '-T1.nrrd";

   m_log = "Applying transformations to T1";
   m_test = "T1Atlas";
   m_outputs.insert("T1Reg", T1Reg); 
   m_argumentsList << "ResampleVolume2" << "T1Atlas" << "T1Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute();

   // Applying transformations to T2
   QString T2Reg = "' + outbase + '-T2.nrrd";

   m_log = "Applying transformations to T2";
   m_test = "T2Atlas";
   m_outputs.insert("T2Reg", T2Reg); 
   m_argumentsList << "ResampleVolume2" << "T2Atlas" << "T2Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute(); 

   if(probabilistic)
   {
      // Applying transformations to the white probability 
      QString whiteReg = "' + outbase + '-white.nrrd";

      m_log = "Applying transformations to the white probability";
      m_outputs.insert("whiteReg", whiteReg); 
      m_argumentsList << "ResampleVolume2" << "whiteAtlas" << "whiteReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(); 

      // Applying transformations to the gray probability 
      QString grayReg = "' + outbase + '-gray.nrrd";

      m_log = "Applying transformations to the gray probability";
      m_outputs.insert("grayReg", grayReg);
      m_argumentsList << "ResampleVolume2" << "grayAtlas" << "grayReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(); 

      // Applying transformations to the CSF probability 
      QString csfReg = "' + outbase + '-csf.nrrd";

      m_log = "Applying transformations to the CSF probability";
      m_outputs.insert("csfReg", csfReg);
      m_argumentsList << "ResampleVolume2" << "csfAtlas" << "csfReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(); 
   }
   
   else
   {
      // Applying transformations to the segmentation
      QString segReg = "' + outbase + '-seg.nrrd";

      m_log = "Applying transformations to the white probability";
      m_outputs.insert("segReg", segReg);
      m_argumentsList << "ResampleVolume2" << "segAtlas" << "segReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(); 
   }
}

void AtlasRegistration::writeRegisterAtlas() // args = (T1Atlas, T2Atlas, output, log)
{
   initializeScript();

   implementStop();
   implementCheckFileExistence();
   initializeLogging(); 
   implementExecute();

   implementRegisterAtlas(0);

   m_script += "if __name__ == '__main__':\n";
   m_script += "\tmain(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6])\n";

   QString script_path = m_processing_dir->filePath("atlasRegistration.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << m_script.toStdString() << std::endl; 
}

void AtlasRegistration::writeRegisterProbabilisticAtlas() // args = (T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, outbase, log)
{
   m_script = ""; 

   initializeScript();

   implementStop();
   implementCheckFileExistence();
   initializeLogging(); 
   implementExecute();

   implementRegisterAtlas(1);

   m_script += "if __name__ == '__main__':\n";
   m_script += "\tmain(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7], sys.argv[8])\n";

   QString script_path = m_processing_dir->filePath("probabilisticAtlasRegistration.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << m_script.toStdString() << std::endl; 
}

void AtlasRegistration::update()
{     
   writeRegisterAtlas();
   writeRegisterProbabilisticAtlas();
}


