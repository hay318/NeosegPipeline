#include "AtlasRegistration.h"

AtlasRegistration::AtlasRegistration(QString module) : Script(module)
{

}

// SET // 
void AtlasRegistration::setAtlasPopulation(std::vector<Atlas> atlasPopulation) 
{
   m_atlasPopulation=atlasPopulation;
}
void AtlasRegistration::setComputingSystem(QString computingSystem)
{
   m_computingSystem=computingSystem;
}
void AtlasRegistration::setNumberOfCores(int nbCores)
{
   m_nbCores=nbCores;
} 
void AtlasRegistration::setAntsParameters(AntsParameters* parameters) 
{
   m_parameters=parameters;
}

// Define Output //  
Atlas AtlasRegistration::defineRegisteredAtlas(Atlas atlas)
{
   Atlas atlasReg = atlas;

   QString atlas_path = m_module_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path); 

   QString T1Reg_name = atlas.name + "_to_" + m_neo.prefix + "-T1.nrrd";
   atlasReg.T1 = atlas_dir->filePath(T1Reg_name); 

   QString T2Reg_name = atlas.name + "_to_" + m_neo.prefix + "-T2.nrrd";
   atlasReg.T2 = atlas_dir->filePath(T2Reg_name);

   if(atlas.probabilistic)
   {
      QString whiteReg_name = atlas.name + "_to_" + m_neo.prefix + "-white.nrrd";
      atlasReg.white = atlas_dir->filePath(whiteReg_name);

      QString grayReg_name = atlas.name + "_to_" + m_neo.prefix + "-gray.nrrd";
      atlasReg.gray = atlas_dir->filePath(grayReg_name);

      QString csfReg_name = atlas.name + "_to_" + m_neo.prefix + "-csf.nrrd";
      atlasReg.csf = atlas_dir->filePath(csfReg_name);
   }

   else
   {
      QString segReg_name = atlas.name + "_to_" + m_neo.prefix + "-seg.nrrd";
      atlasReg.seg = atlas_dir->filePath(segReg_name);
   }

   return atlasReg;
}

std::vector<Atlas> AtlasRegistration::defineRegisteredAtlasPopulation()
{
   std::vector<Atlas> atlasPopulationRegistered;
   Atlas atlas;

   std::vector<Atlas>::const_iterator it;

   for (it = m_atlasPopulation.begin(); it != m_atlasPopulation.end(); ++it)
   {  
      atlas=defineRegisteredAtlas(*it);
      atlasPopulationRegistered.push_back(atlas);
   }   

   return atlasPopulationRegistered;
}


// Creating Directories //
void AtlasRegistration::createAtlasDirectories()
{
   std::vector<Atlas>::iterator it;

   for (it = m_atlasPopulation.begin(); it != m_atlasPopulation.end(); ++it)
   {  
      if (!m_module_dir->exists((*it).name))
      { 
         m_module_dir->mkdir((*it).name);
      }
   }
}

void AtlasRegistration::initializeScript(QString &script)
{
   definePython(script);
   importGeneralModules(script);
  
   script += "import time\n";
   script += "import array\n";
   script += "from collections import namedtuple\n\n";

   defineExecutable(script, "ANTS");
   defineExecutable(script, "ResampleVolume2");
   defineExecutable(script, "ImageMath");
   defineExecutable(script, "unu");
   defineExecutable(script, "N4ITKBiasFieldCorrection");
 
   script += "runningRegistrations = None\n"; 
   script += "runningProcess = None\n";  
   script += "logFile = None\n\n";  
}

void AtlasRegistration::implementExecute(QString &script)
{
   script += "def execute(args):\n";
   script += "\tlogFile.write(subprocess.list2cmdline(args))\n";
   script += "\trunningProcess = subprocess.Popen(args,stdout=logFile,stderr=logFile)\n";
   script += "\tlogging.info(runningProcess)\n";
   script += "\trunningProcess.wait()\n\n";
}

void AtlasRegistration::implementRegisterAtlas(QString &script, bool probabilistic)
{   
   //QString log = variable("log");

   if(probabilistic)
   {
      script += "def main(T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, outbase, log):\n";
   } 
   else
   {
      script += "def main(T1Atlas, T2Atlas, segAtlas, outbase, log):\n";
   }

   script += "\tsignal.signal(signal.SIGINT, stop)\n";
   script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   script += "\tglobal logFile\n";
   script += "\tlogFile=open(log, \"w\") \n\n";

   script += "\tT1 = '" + m_neo.T1 + "'\n";
   script += "\tT2 = '" + m_neo.T2 + "'\n";

   if(m_parameters->getUsingSmoothedMask())
   {
      QString smoothedMask_name = m_prefix + "mask-smoothed" + m_suffix + ".nrrd";
      QString smoothedMask_path = m_module_dir->filePath(smoothedMask_name) ;

      m_log = "- Smoothing brain mask"; 
      m_inputs.insert("mask", m_neo.mask);
      m_outputs.insert("smoothedMask", smoothedMask_path); 
      m_argumentsList << "ImageMath" << "mask" << "'-smooth'" << "'-gauss'" << "'-size'" << "'1'" << "'-outfile'" << "smoothedMask"; 
      execute(script); 
   }

   QString modality1, modality2; 

   if(m_parameters->getImageMetric1()=="Mutual Information")
   {
      modality1 = "MI[' + T1 + ',' + T1Atlas + '," + QString::number(m_parameters->getWeight1()) + "," + QString::number(m_parameters->getRadius1()) + "]";
      modality2 = "MI[' + T2 + ',' + T2Atlas + '," + QString::number(m_parameters->getWeight2()) + "," + QString::number(m_parameters->getRadius2()) + "]";
   }

   else
   {
      modality1 = m_parameters->getImageMetric1() + "[' + T1 + ',' + T1Atlas + '," + QString::number(m_parameters->getWeight1()) + "," + QString::number(m_parameters->getRadius1()) + "]";
      modality2 = m_parameters->getImageMetric2() + "[' + T2 + ',' + T2Atlas + '," + QString::number(m_parameters->getWeight2()) + "," + QString::number(m_parameters->getRadius2()) + "]";
   }
   QString iterations = QString::number(m_parameters->getIterationsJ()) + "x" + QString::number(m_parameters->getIterationsK()) + "x" + QString::number(m_parameters->getIterationsL());
   QString transformation = m_parameters->getTransformationType() + "[" + QString::number(m_parameters->getGradientStepLength()) + "]"; //"," + QString::number(m_parameters->getNumberOfTimeSteps()) + "," + QString::number(m_parameters->getDeltaTime()) + "]";
   QString regularization = m_parameters->getRegularizationType() + "[" + QString::number(m_parameters->getGradientFieldSigma()) + "," + QString::number(m_parameters->getDeformationFieldSigma()) + "]";
   QString output = "' + outbase + '_.nii.gz";

   QString affine = "' + outbase + '_Affine.txt";
   QString warp = "' + outbase + '_Warp.nii.gz";

   m_log = "Calculating transformations";
   m_inputs.insert("modality1", modality1);
   m_inputs.insert("modality2", modality2);
   m_inputs.insert("iterations", iterations); 
   m_inputs.insert("transformation", transformation);
   m_inputs.insert("regularization", regularization);  
   m_inputs.insert("output", output );
   m_outputs.insert("affine", affine);
   m_outputs.insert("warp", warp);  

   if(m_parameters->getUsingMask())
   {
      m_inputs.insert("mask", m_neo.mask);
      m_argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization" << "'-x'" << "mask"; 
   }
   else    if(m_parameters->getUsingSmoothedMask())
   {
      m_argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization" << "'-x'" << "smoothedMask";       
   }
   else
   {
      m_argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization";
   }

   execute(script); 

   // Normalizing affine file
   m_log = "Normalizing affine file";
   m_argumentsList << "'text_subst.pl'" << "'MatrixOffsetTransformBase_double_3_3'" << "'AffineTransform_double_3_3'" << "affine"; 
   execute(script);

   // Applying transformations to T1
   QString T1Reg = "' + outbase + '-T1.nrrd";

   m_log = "Applying transformations to T1";
   m_outputs.insert("T1Reg", T1Reg); 
   m_argumentsList << "ResampleVolume2" << "T1Atlas" << "T1Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute(script);

   // Applying transformations to T2
   QString T2Reg = "' + outbase + '-T2.nrrd";

   m_log = "Applying transformations to T2";
   m_outputs.insert("T2Reg", T2Reg); 
   m_argumentsList << "ResampleVolume2" << "T2Atlas" << "T2Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute(script); 

   if(probabilistic)
   {
      // Applying transformations to the white probability 
      QString whiteReg = "' + outbase + '-white.nrrd";

      m_log = "Applying transformations to the white probability";
      m_outputs.insert("whiteReg", whiteReg); 
      m_argumentsList << "ResampleVolume2" << "whiteAtlas" << "whiteReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(script); 

      // Applying transformations to the gray probability 
      QString grayReg = "' + outbase + '-gray.nrrd";

      m_log = "Applying transformations to the gray probability";
      m_outputs.insert("grayReg", grayReg);
      m_argumentsList << "ResampleVolume2" << "grayAtlas" << "grayReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(script); 

      // Applying transformations to the CSF probability 
      QString csfReg = "' + outbase + '-csf.nrrd";

      m_log = "Applying transformations to the CSF probability";
      m_outputs.insert("csfReg", csfReg);
      m_argumentsList << "ResampleVolume2" << "csfAtlas" << "csfReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(script); 
   }
   
   else
   {
      // Applying transformations to the segmentation
      QString segReg = "' + outbase + '-seg.nrrd";

      m_log = "Applying transformations to the white probability";
      m_outputs.insert("segReg", segReg);
      m_argumentsList << "ResampleVolume2" << "segAtlas" << "segReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(script); 
   }
}

void AtlasRegistration::writeRegisterAtlas() // args = (T1Atlas, T2Atlas, output, log)
{
   QString registerAtlas;

   initializeScript(registerAtlas);

   implementStop(registerAtlas);
   implementCheckFileExistence(registerAtlas);
   implementExecute(registerAtlas);
   implementRegisterAtlas(registerAtlas, 0);

   registerAtlas += "if __name__ == '__main__':\n";
   registerAtlas += "\tmain(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])\n";

   QString script_path = m_processing_dir->filePath("registerAtlas.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerAtlas.toStdString() << std::endl; 
}

void AtlasRegistration::writeRegisterProbabilisticAtlas() // args = (T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, outbase, log)
{
   QString registerProbabilisticAtlas;

   initializeScript(registerProbabilisticAtlas);

   implementStop(registerProbabilisticAtlas);
   implementCheckFileExistence(registerProbabilisticAtlas);
   implementExecute(registerProbabilisticAtlas);
   implementRegisterAtlas(registerProbabilisticAtlas, 1);

   registerProbabilisticAtlas += "if __name__ == '__main__':\n";
   registerProbabilisticAtlas += "\tmain(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7])\n";

   QString script_path = m_processing_dir->filePath("registerProbabilisticAtlas.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << registerProbabilisticAtlas.toStdString() << std::endl; 
}

void AtlasRegistration::defineRegisterAtlasParameters(QString &script, Atlas atlas)
{
   script += "\t# Registration " + atlas.name + " to " + m_neo.prefix + "\n"; 

   script += "\tT1Atlas = '" + atlas.T1 + "'\n";
   script += "\tT2Atlas = '" + atlas.T2 + "'\n";

   if(atlas.probabilistic)
   {
      script += "\twhiteAtlas = '" + atlas.white + "'\n";
      script += "\tgrayAtlas = '" + atlas.gray + "'\n";
      script += "\tcsfAtlas = '" + atlas.csf + "'\n";
   }
   
   else
   {
      script += "\tsegAtlas = '" + atlas.seg + "'\n";
   }

   QString atlas_path = m_module_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path);

   QString output_name = atlas.name + "_to_" + m_neo.prefix;     
   QString output = atlas_dir->filePath(output_name);
   script += "\toutput = '" + output + "'\n";

   QString log_name = atlas.name + "_to_" + m_neo.prefix + ".log";     
   QString log = atlas_dir->filePath(log_name);
   script += "\tlog = '" + log + "'\n\n";
}

void AtlasRegistration::submitRegisterAtlasJob(QString &script)
{
   /*QString bsub = "bsub -n 1 " ;

   QString executable = "python " + variable("atlasRegistration_script") + " " + variable("T1Atlas") + " " + variable("T2Atlas") + " " + variable("segAtlas") + " " + variable("output") + " " + variable("log");

   QString command = bsub + " " + executable; 
   //script += "\tos.system(\"" + command + "\") \n\n";*/
}

void AtlasRegistration::executeRegisterAtlasProcess(QString &script, Atlas atlas, int i) 
{
   QString QString_i; 
   QString_i.setNum(i);

   script += "\twhile len(runningRegistrations) >= nbRegMax : \n";
   script += "\t\tcheckrunningRegistrations(runningRegistrations)\n";
   script += "\t\ttime.sleep(10)\n";

   QString process_name = "process_" + QString_i;
   QString registration_name = "registration_" + QString_i;

   QString command;

   if(atlas.probabilistic)
   {
      command = "['python', probabilisticAtlasRegistration_script, T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, output, log]";
   }
   
   else
   {
      command = "['python', atlasRegistration_script, T1Atlas, T2Atlas, segAtlas, output, log]";
   }

   script += "\tlogging.info('Registering " + atlas.name + "...')\n";
   script += "\t" + process_name + " = subprocess.Popen(" + command + ")\n";
   script += "\t" + registration_name + " = registration('" + atlas.name + "', " + process_name + ")\n";
   script += "\trunningRegistrations.append(" + registration_name + ")\n\n";
}


void AtlasRegistration::implementRegisterAtlasPopulation(QString &script)
{
   script += "import registerAtlas\n";
   script += "import registerProbabilisticAtlas\n";

   int i;
   script += "def stop(signal, frame):\n";
   script += "\tprint 'Signal received'\n";
   script += "\tfor registration in runningRegistrations:\n";
   script += "\t\t(registration.process).terminate()\n";
   script += "\tsys.exit(0)\n\n";

   script += "def checkrunningRegistrations(runningRegistrations):\n";
   script += "\tfor registration in runningRegistrations :\n";
   script += "\t\tif (registration.process).poll()==0:\n";
   script += "\t\t\tlogging.info('Registering %s done' %registration.name)\n";
   script += "\t\t\trunningRegistrations.remove(registration)\n\n";

   script += "def run():\n";

   script += "\tlogging.info('=== Atlas Registration ===')\n";
   script += "\tlogging.debug('')\n";

   script += "\tglobal runningRegistrations\n";
   script += "\tsignal.signal(signal.SIGINT, stop)\n";
   script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   QString atlasRegistration_script = m_processing_dir->filePath("registerAtlas.py");
   script += "\tatlasRegistration_script = '" + atlasRegistration_script + "'\n";

   QString probabilisticAtlasRegistration_script = m_processing_dir->filePath("registerProbabilisticAtlas.py");
   script += "\tprobabilisticAtlasRegistration_script = '" + probabilisticAtlasRegistration_script + "'\n\n";

   script += "\tos.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS'] = '1' \n";
   script += "\tcheck = int(os.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS']) \n\n";

   script += "\tnbRegMax = " + QString::number(m_nbCores) +  "\n";
   script += "\trunningRegistrations = []\n\n";

   script += "\tregistration = namedtuple('registration', 'name process')\n\n";

   std::vector<Atlas>::iterator it;
   for (it = m_atlasPopulation.begin(); it != m_atlasPopulation.end(); ++it)
   {  
      i = std::distance(m_atlasPopulation.begin(), it);

      Atlas atlas= *(it);

      defineRegisterAtlasParameters(script, atlas);

      if (m_computingSystem == "Killdevil")
      {
         submitRegisterAtlasJob(script);
      }

      if (m_computingSystem == "Local")
      {
         i = std::distance(m_atlasPopulation.begin(), it);
         executeRegisterAtlasProcess(script, *it, i);
      }
   }
   script += "\texit_codes = [(registration.process).wait() for registration in runningRegistrations]\n";
}

void AtlasRegistration::writeRegisterAtlasPopulation()
{
   QString registerAtlasPopulation;

   initializeScript(registerAtlasPopulation);
   implementRegisterAtlasPopulation(registerAtlasPopulation);
   writeScript(registerAtlasPopulation);
}

void AtlasRegistration::update()
{     
   defineRegisteredAtlasPopulation();
   createAtlasDirectories();
   writeRegisterAtlas();
   writeRegisterProbabilisticAtlas();
   writeRegisterAtlasPopulation();
}

std::vector<Atlas> AtlasRegistration::getOutput()
{
   return defineRegisteredAtlasPopulation();
}

