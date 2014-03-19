#include "DtiRegistration.h"

DtiRegistration::DtiRegistration(QString module) : Script(module)
{
   m_registering_suffix = "-registered";
}

void DtiRegistration::initializeScript()
{
   definePython(m_script);
   importGeneralModules(m_script);

   m_script += "import time \n";
   m_script += "import array \n\n";

   defineExecutable(m_script, "ANTS");
   defineExecutable(m_script, "ResampleVolume2");
   defineExecutable(m_script, "ITKTransformTools");
   defineExecutable(m_script, "dtiestim");
   defineExecutable(m_script, "ImageMath");
   defineExecutable(m_script, "bet2");
   defineExecutable(m_script, "dtiprocess");
   defineExecutable(m_script, "HistogramMatching");

   m_script += "runningProcess = None\n\n";  
}

void DtiRegistration::upsample(QString image)
{
   QStringList argumentsList;

   QString grid_name = m_neo.prefix + "-" + image + "-grid.nrrd";
   QString grid_path = m_module_dir->filePath(grid_name); 
   m_script += "\tgrid" + image + " = '" + grid_path + "'\n"; 

   m_script += "\tif checkFileExistence(grid" + image + ")==False:\n";

   argumentsList << "ITKTransformTools" << "'size'" << image << "'-'" << "'--grid'" << "grid" + image << "'--spacing'" << "'1'" << "'1'" << "'1'"; 
   m_script += "\t\tlogging.info('- Computing the grid of the image upsampled...')\n";
   m_script += "\t\targs = " + listToString(argumentsList) + "\n";
   m_script += "\t\texecute(args)\n";

   QString upsampledImage_name = m_neo.prefix + "-" + image + "-upsampled.nrrd";
   QString upsampledImage_path = m_module_dir->filePath(upsampledImage_name); 
   m_script += "\tupsampled" + image + " = '" + upsampledImage_path + "'\n"; 

   m_script += "\tif checkFileExistence(upsampled" + image + ")==False:\n";

   argumentsList.clear();
   argumentsList << "ResampleVolume2" << "'-R'" << "grid" + image << "'-i'" << "'linear'" << image << "upsampled" + image; 
   m_script += "\t\tlogging.info('- Upsampling the image...')\n";
   m_script += "\t\targs = " + listToString(argumentsList) + "\n";
   m_script += "\t\texecute(args)\n";
}

void DtiRegistration::generateDTI()
{
   QStringList argumentsList;

   QString DTI_name = m_neo.prefix + "-DTI.nrrd";
   QString DTI_path = m_module_dir->filePath(DTI_name); 
   m_script += "\tDTI = '" + DTI_path + "'\n";

   m_script += "\tif checkFileExistence(DTI)==False:\n";

   argumentsList << "dtiestim" << "'--dwi_image'" << "DWI" << "'--B0'" << "b0" << "'--tensor_output'" << "DTI"; 
   m_script += "\t\targs = " + listToString(argumentsList) + "\n";
   m_script += "\t\texecute(args)\n";
}

void DtiRegistration::skullStripb0()
{
   QStringList argumentsList;

   QString b0Nifti_name = m_neo.prefix + "-b0.nii.gz";
   QString b0Nifti_path = m_module_dir->filePath(b0Nifti_name); 
   m_script += "\tb0Nifti = '" + b0Nifti_path + "'\n";   

   m_script += "\tif checkFileExistence(b0Nifti)==False:\n";

   argumentsList << "ResampleVolume2" << "b0" << "b0Nifti"; 
   m_script += "\t\tlogging.info('- Converting the b0 in nifti...')\n";
   m_script += "\t\targs = " + listToString(argumentsList) + "\n";
   m_script += "\t\texecute(args)\n";

   QString b0Mask_name = m_neo.prefix + "-b0_mask.nii.gz";
   QString b0Mask_path = m_module_dir->filePath(b0Mask_name); 
   m_script += "\tb0Mask = '" + b0Mask_path + "'\n";

   QString strippedb0_name = m_neo.prefix + "-b0.nii.gz";
   QString strippedb0_path = m_module_dir->filePath(strippedb0_name); 
   m_script += "\tstrippedb0 = '" + strippedb0_path + "'\n";

   m_script += "\tif checkFileExistence(strippedb0)==False or checkFileExistence(b0Mask)==False:\n";

   QString outbase_name = m_neo.prefix + "-b0";
   QString outbase_path = m_module_dir->filePath(outbase_name); 
   m_script += "\t\toutbase = '" + outbase_path + "'\n";

   argumentsList.clear();
   argumentsList << "bet2" << "b0Nifti" << "outbase" << "'-m'";
   m_script += "\t\tlogging.info('- Skull-stripping the b0...')\n";
   m_script += "\t\targs = " + listToString(argumentsList) + "\n";
   m_script += "\t\texecute(args)\n";
}

void DtiRegistration::skullStripDTI()
{
   QStringList argumentsList;

   QString strippedDTI_name = m_neo.prefix + "-DTI-stipped.nrrd";
   QString strippedDTI_path = m_module_dir->filePath(strippedDTI_name); 
   m_script += "\tstrippedDTI = '" + strippedDTI_path + "'\n";

   argumentsList << "ImageMath" << "DTI" << "'-mask'" << "b0Mask" << "'-outfile'" << "strippedDTI"; 
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";
}

void DtiRegistration::generateFA()
{
   QStringList argumentsList;

   QString FA_name = m_neo.prefix + "-FA.nrrd"; 
   QString FA_path = m_module_dir->filePath(FA_name); 
   m_script += "\tFA = '" + FA_path + "'\n";

   argumentsList << "dtiprocess" << "'--dti_image'" << "strippedDTI" << "'--fa_output'" << "FA"; 
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";
}

void DtiRegistration::generateAD()
{
   QStringList argumentsList;

   QString AD_name = m_neo.prefix + "-AD.nrrd";
   QString AD_path = m_module_dir->filePath(AD_name); 
   m_script += "\tAD = '" + AD_path + "'\n";

   argumentsList << "dtiprocess" << "'--dti_image'" << "strippedDTI" << "'--lambda1_output'" << "AD"; 
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";
}

void DtiRegistration::calculateTransformations()
{
   QStringList argumentsList;

   m_script += "\tmodality1 = 'CC[' + T2 + ',' + strippedb0 + ',1,2]'\n"; 
   m_script += "\tmodality2 = 'CC[' + T2 + ',' + upsampledAD + ',1,2]'\n"; 

   QString output_name = "DTI_to_" + m_neo.prefix + "-.nii.gz";
   QString output_path = m_module_dir->filePath(output_name); 
   m_script += "\toutput = '" + output_path + "'\n";

   QString warp_name = "DTI_to_" + m_neo.prefix + "-Warp.nii.gz";
   QString warp_path = m_module_dir->filePath(warp_name); 
   m_script += "\twarp = '" + warp_path + "'\n";

   QString affine_name = "DTI_to_" + m_neo.prefix + "-Affine.txt";
   QString affine_path = m_module_dir->filePath(affine_name); 
   m_script += "\taffine = '" + affine_path + "'\n";

   m_script += "\tif checkFileExistence(affine)==False or checkFileExistence(warp)==False:\n";
   argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "'30x20x10'" << "'-r'" << "'Gauss[5,5]'" << "'-t'" << "'SyN[0.125]'"; 
   m_script += "\t\tlogging.info('- Calculating transformations...')\n";
   m_script += "\t\targs = " + listToString(argumentsList) + "\n";
   m_script += "\t\texecute(args)\n";

   argumentsList.clear();
   argumentsList << "'text_subst.pl'" << "'MatrixOffsetTransformBase_double_3_3'" << "'AffineTransform_double_3_3'" << "affine"; 
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n\n";
}

void DtiRegistration::applyTransformations(QString image)
{
   QStringList argumentsList;

   QString registeredImage_name = m_neo.prefix + "-" + image + "-registered.nrrd";
   QString registeredImage_path = m_module_dir->filePath(registeredImage_name); 
   m_script += "\tregistered" + image + " = '" + registeredImage_path + "'\n";

   argumentsList << "ResampleVolume2" << "upsampled" + image << "registered" + image << "'--Reference'" << "T1" << "'-i'" << "'linear'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine"; 
   m_script += "\tlogging.info('- Applying transformations to the " + image + "...')\n";
   m_script += "\targs = " + listToString(argumentsList) + "\n";
   m_script += "\texecute(args)\n";

   if(image == "FA")
   {
      m_neo.FA = registeredImage_path;
   }
   if(image == "AD")
   {
      m_neo.AD = registeredImage_path;
   }
}

void DtiRegistration::implementRun()
{
   QStringList argumentsList;
   
   m_script += "def run():\n\n";

   m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
   m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   m_script += "\tlogging.info('=== DTI Registration ===')\n";
   m_script += "\tlogging.debug('')\n";

   m_script += "\tT1 = '" + m_neo.T1 + "'\n";
   m_script += "\tT2 = '" + m_neo.T2 + "'\n";
   m_script += "\tDWI = '" + m_neo.DWI + "'\n";
   m_script += "\tb0 = '" + m_neo.b0 + "'\n";

   m_script += "\n\t# Upsample b0 #\n";
   m_script += "\tlogging.info('Upsampling the b0 :')\n";
   m_script += "\tlogging.debug('')\n";
   upsample("b0"); 
   m_script += "\tlogging.debug('')\n";

   m_script += "\n\t# Generate DTI #\n";
   m_script += "\tlogging.info('Generating the DTI...')\n";
   generateDTI(); 

   m_script += "\n\t# Skull-Strip b0 #\n";
   m_script += "\tlogging.info('Skull-stripping the b0 :')\n";
   skullStripb0(); 

   m_script += "\n\t# Skull-Strip DTI #\n";
   m_script += "\tlogging.info('Skull-stripping the DTI...')\n";
   skullStripDTI(); 

   m_script += "\n\t# Generate FA #\n";  
   m_script += "\tlogging.info('Generating the FA...')\n";
   generateFA(); 

   m_script += "\n\t# Upsample FA #\n";
   m_script += "\tlogging.info('Upsampling the FA :')\n";
   m_script += "\tlogging.debug('')\n";
   upsample("FA");
   m_script += "\tlogging.debug('')\n"; 

   m_script += "\n\t# Generate AD #\n"; 
   m_script += "\tlogging.info('Generating the AD...')\n";
   generateAD(); 

   m_script += "\n\t# Upsample AD #\n";
   m_script += "\tlogging.info('Upsampling the AD :')\n";
   m_script += "\tlogging.debug('')\n"; 
   upsample("AD"); 
   m_script += "\tlogging.debug('')\n";

   m_script += "\n\t# Registering FA and AD #\n";
   m_script += "\tlogging.info('Registering the FA and the AD :')\n";
   calculateTransformations(); 
   applyTransformations("FA"); 
   applyTransformations("AD"); 
}

void DtiRegistration::update()
{
   initializeScript();

   implementStop(m_script);
   implementCheckFileExistence(m_script);
   implementExecute(m_script); 
   implementExecutePipe(m_script); 

   implementRun();

   writeScript(m_script);
}

Neo DtiRegistration::getOutput()
{
   return m_neo;
}


