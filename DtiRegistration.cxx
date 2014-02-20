#include "DtiRegistration.h"

DtiRegistration::DtiRegistration()
{

}

void DtiRegistration::setNeo(Neo neo) {m_neo=neo;}

void DtiRegistration::setOutputPath(QString outputPath) 
{
   m_output_dir = new QDir(outputPath); 

   m_DTIRegistration_path = m_output_dir->filePath("DTIRegistration");
   m_DTIRegistration_dir = new QDir(m_DTIRegistration_path);
}

void DtiRegistration::setExecutablePaths(ExecutablePaths* executables) {m_executables=executables;}

void DtiRegistration::defineRegisteredDTI()
{
   m_registeredNeo = m_neo; 

   QString FAReg_name = "FA-registered.nrrd";
   m_registeredNeo.FA = m_DTIRegistration_dir->filePath(FAReg_name); 

   QString ADReg_name = "AD-registered.nrrd";
   m_registeredNeo.AD = m_DTIRegistration_dir->filePath(ADReg_name); 
}

// Checking results // 

bool DtiRegistration::checkRegisteredDTI()
{
   QFile* FAReg_file = new QFile(m_registeredNeo.FA);
   QFile* ADReg_file = new QFile(m_registeredNeo.AD);

   if( FAReg_file->exists() && ADReg_file->exists() )
   {
      return true;
   } 
   return false; 
}


void DtiRegistration::createDirectory()
{
   m_output_dir->mkpath("DTIRegistration");
   m_DTIRegistration_path = m_output_dir->filePath("DTIRegistration");
   m_DTIRegistration_dir = new QDir(m_DTIRegistration_path);
}

// Writing Scripts //

QString DtiRegistration::variable(QString variable_name) {return "\" + " + variable_name + " + \"";}

QString DtiRegistration::str(QString str) {return "\"" + str + "\"";}

QString DtiRegistration::listToString(QStringList argumentsList)
{
   QString arguments = argumentsList[0];

   QStringList::const_iterator it;
   for (it = argumentsList.constBegin()+1; it != argumentsList.constEnd(); ++it)
   {
      arguments += "," + (*it) + "";
   }

      return arguments; 
}

void DtiRegistration::initializeScript()
{
   m_script += "#!/usr/bin/env python\n";

   // Import the librairies needed  
   m_script += "import os \n";
   m_script += "import sys \n";
   m_script += "import signal \n";
   m_script += "import logging \n";
   m_script += "import subprocess \n";
   m_script += "import time \n";
   m_script += "import array \n\n";

   m_script += "ANTS = " + str(m_executables->getExecutablePath("ANTS")) + "\n";
   m_script += "ResampleVolume2 = " + str(m_executables->getExecutablePath("ResampleVolume2")) + "\n";
   m_script += "ITKTransformTools = " + str(m_executables->getExecutablePath("ITKTransformTools")) + "\n";
   m_script += "dtiestim = " + str(m_executables->getExecutablePath("dtiestim")) + "\n";
   m_script += "ImageMath = " + str(m_executables->getExecutablePath("ImageMath")) + "\n";
   m_script += "bet2 = " + str(m_executables->getExecutablePath("bet2")) + "\n";
   m_script += "dtiprocess = " + str(m_executables->getExecutablePath("dtiprocess")) + "\n\n";
}

void DtiRegistration::implementLogStdoutAndStderr()
{
   m_script += "def logStdoutAndStderr(runningProcess):\n";

   m_script += "\twhile True:\n";
   m_script += "\t\tline = runningProcess.stdout.readline()\n";
   m_script += "\t\tif line:\n";
   m_script += "\t\t\tlogging.info(line.rstrip())\n";
   m_script += "\t\tif not line: break\n";

   m_script += "\twhile True:\n";
   m_script += "\t\tline = runningProcess.stderr.readline()\n";
   m_script += "\t\tif line:\n";
   m_script += "\t\t\tlogging.error(line.rstrip())\n";
   m_script += "\t\tif not line: break\n";

   m_script += "\trunningProcess.wait()\n\n";
}

void DtiRegistration::implementLogStderr()
{
   m_script += "def logStderr(runningProcess):\n";

   m_script += "\twhile True:\n";
   m_script += "\t\tline = runningProcess.stderr.readline()\n";
   m_script += "\t\tif line:\n";
   m_script += "\t\t\tlogging.error(line.rstrip())\n";
   m_script += "\t\tif not line: break\n";

   m_script += "\trunningProcess.wait()\n\n";
}

void DtiRegistration::upsample(QString image)
{
   QStringList argumentsList;

   QString grid_name = image + "-grid.nrrd";
   QString grid_path = m_DTIRegistration_dir->filePath(grid_name); 
   m_script += "\tgrid" + image + " = " + str(grid_path) + "\n"; 

   argumentsList << "ITKTransformTools" << "'size'" << image << "'-'" << "'--grid'" << "grid" + image << "'--spacing'" << "'1'" << "'1'" << "'1'"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   m_script += "\tlogStdoutAndStderr(runningProcess)\n\n";

   QString upsampledImage_name = image + "-upsampled.nrrd";
   QString upsampledImage_path = m_DTIRegistration_dir->filePath(upsampledImage_name); 
   m_script += "\tupsampled" + image + " = " + str(upsampledImage_path) + "\n"; 

   argumentsList.clear();
   argumentsList << "ResampleVolume2" << "'-R'" << "grid" + image << "'-i'" << "'linear'" << image << "upsampled" + image; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";
}

void DtiRegistration::generateDTI()
{
   QStringList argumentsList;

   QString DTI = m_DTIRegistration_dir->filePath("DTI.nrrd"); 
   m_script += "\tDTI = " + str(DTI) + "\n";

   argumentsList << "dtiestim" << "'--dwi_image'" << "DWI" << "'--B0'" << "b0" << "'--tensor_output'" << "DTI"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";
}

void DtiRegistration::skullStripb0()
{
   QStringList argumentsList;

   QString b0Nifti = m_DTIRegistration_dir->filePath("b0.nii.gz"); 
   m_script += "\tb0Nifti = " + str(b0Nifti) + "\n";   

   argumentsList << "ResampleVolume2" << "b0" << "b0Nifti"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";

   QString outbase = m_DTIRegistration_dir->filePath("b0"); 
   m_script += "\toutbase = " + str(outbase) + "\n";

   QString strippedb0 = m_DTIRegistration_dir->filePath("b0.nii.gz"); 
   m_script += "\tstrippedb0 = " + str(strippedb0) + "\n";

   QString b0Mask = m_DTIRegistration_dir->filePath("b0_mask.nii.gz"); 
   m_script += "\tb0Mask = " + str(b0Mask) + "\n";

   argumentsList.clear();
   argumentsList << "bet2" << "b0Nifti" << "outbase" << "'-m'";
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";
}

void DtiRegistration::skullStripDTI()
{
   QStringList argumentsList;

   QString strippedDTI = m_DTIRegistration_dir->filePath("DTI-stripped.nrrd"); 
   m_script += "\tstrippedDTI = " + str(strippedDTI) + "\n";

   argumentsList << "ImageMath" << "DTI" << "'-mask'" << "b0Mask" << "'-outfile'" << "strippedDTI"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";
}

void DtiRegistration::generateFA()
{
   QStringList argumentsList;

   QString FA = m_DTIRegistration_dir->filePath("FA.nrrd"); 
   m_script += "\tFA = " + str(FA) + "\n";

   argumentsList << "dtiprocess" << "'--dti_image'" << "strippedDTI" << "'--fa_output'" << "FA"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";
}

void DtiRegistration::generateAD()
{
   QStringList argumentsList;

   QString AD = m_DTIRegistration_dir->filePath("AD.nrrd"); 
   m_script += "\tAD = " + str(AD) + "\n";

   argumentsList << "dtiprocess" << "'--dti_image'" << "strippedDTI" << "'--lambda1_output'" << "AD"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";
}

void DtiRegistration::calculateTransformations()
{
   QStringList argumentsList;

   m_script += "\tmodality1 = 'CC[' + T2 + ',' + strippedb0 + ',1,2]'\n"; 
   m_script += "\tmodality2 = 'CC[' + T2 + ',' + upsampledAD + ',1,2]'\n"; 

   QString output = m_DTIRegistration_dir->filePath("DTI_to_neo-.nii.gz"); 
   m_script += "\toutput = " + str(output) + "\n";

   QString warp = m_DTIRegistration_dir->filePath("DTI_to_neo-Warp.nii.gz"); 
   m_script += "\twarp = " + str(warp) + "\n";

   QString affine = m_DTIRegistration_dir->filePath("DTI_to_neo-Affine.txt"); 
   m_script += "\taffine = " + str(affine) + "\n";

   m_script += "\tif checkFileExistence(affine)==False or checkFileExistence(warp)==False:\n";
   argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "'30x20x10'" << "'-r'" << "'Gauss[5,5]'" << "'-t'" << "'SyN[0.125]'"; 
   m_script += "\t\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   m_script += "\t\tlogStdoutAndStderr(runningProcess)\n\n";
}

void DtiRegistration::applyTransformations()
{
   QStringList argumentsList;

   argumentsList << "'text_subst.pl'" << "'MatrixOffsetTransformBase_double_3_3'" << "'AffineTransform_double_3_3'" << "affine"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   m_script += "\tlogStdoutAndStderr(runningProcess)\n\n";

   QString registeredAD = m_DTIRegistration_dir->filePath("AD-registered.nrrd"); 
   m_script += "\tregisteredAD = " + str(registeredAD) + "\n";

   argumentsList.clear();
   argumentsList << "ResampleVolume2" << "upsampledAD" << "registeredAD" << "'--Reference'" << "T1" << "'-i'" << "'linear'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";

   m_neo.AD = registeredAD;

   QString registeredFA = m_DTIRegistration_dir->filePath("FA-registered.nrrd"); 
   m_script += "\tregisteredFA = " + str(registeredFA) + "\n";

   argumentsList.clear();
   argumentsList << "ResampleVolume2" << "upsampledFA" << "registeredFA" << "'--Reference'" << "T1" << "'-i'" << "'linear'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine"; 
   m_script += "\trunningProcess = subprocess.Popen([" + listToString(argumentsList) + "],stderr=subprocess.PIPE)\n";
   m_script += "\tlogStderr(runningProcess)\n\n";

   m_neo.FA = registeredFA;
}


void DtiRegistration::implementRegisterDTI()
{
   QStringList argumentsList;

   m_script += "def signal_handler(signal, frame):\n";
   m_script += "\tprint '*************** Signal received! ******************'\n";
   m_script += "\tprint runningProcess\n";
   m_script += "\tprint runningProcess.pid\n";
   m_script += "\trunningProcess.terminate()\n";
   m_script += "\tsys.exit(0)\n\n";

   m_script += "def checkFileExistence(fileName):\n";
   m_script += "\ttry:\n";
   m_script += "\t\twith open(fileName):\n";
   m_script += "\t\t\treturn True\n";
   m_script += "\texcept IOError:\n";
   m_script += "\t\treturn False\n\n";
   
   m_script += "signal.signal(signal.SIGINT, signal_handler)\n";
   m_script += "signal.signal(signal.SIGTERM, signal_handler)\n\n";

   m_script += "def run():\n\n";

   m_script += "\tT1 = " + str(m_neo.T1) + "\n";
   m_script += "\tT2 = " + str(m_neo.T2) + "\n";
   m_script += "\tDWI = " + str(m_neo.DWI) + "\n";
   m_script += "\tb0 = " + str(m_neo.b0) + "\n\n";

   m_script += "\t# Upsample b0 #\n";
   upsample("b0"); 

   m_script += "\t# Generate DTI #\n";
   generateDTI(); 

   m_script += "\t# Skull-Strip b0 #\n";
   skullStripb0(); 

   m_script += "\t# Skull-Strip DTI #\n";
   skullStripDTI(); 

   m_script += "\t# Generate FA #\n";  
   generateFA(); 

   m_script += "\t# Upsample FA #\n";
   upsample("FA"); 

   m_script += "\t# Generate AD #\n";  
   generateAD(); 

   m_script += "\t# Upsample AD #\n";
   upsample("AD"); 

   m_script += "\t# Calculate Transformations #\n";
   calculateTransformations(); 

   m_script += "\t# Apply Transformations #\n";
   applyTransformations(); 
}

void DtiRegistration::writeRegisterDTI()
{
   initializeScript();

   implementLogStdoutAndStderr(); 
   implementLogStderr(); 

   implementRegisterDTI();

   QString script_path = m_output_dir->filePath("registerDTI.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << m_script.toStdString() << std::endl;   

   script_stream->close();
}

void DtiRegistration::update()
{
   createDirectory();       
   writeRegisterDTI();
}

Neo DtiRegistration::getOutput() {return m_registeredNeo;}


