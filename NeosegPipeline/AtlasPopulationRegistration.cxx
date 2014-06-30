#include "AtlasPopulationRegistration.h"

AtlasPopulationRegistration::AtlasPopulationRegistration(QString module) : Script(module)
{

}

// SET // 
void AtlasPopulationRegistration::setAtlasPopulation(std::vector<Atlas> atlasPopulation) 
{
   m_atlasPopulation=atlasPopulation;
}
void AtlasPopulationRegistration::setComputingSystem(QString computingSystem)
{
   m_computingSystem=computingSystem;
}
void AtlasPopulationRegistration::setAntsParameters(AntsParameters* parameters) 
{
   m_parameters = parameters;
}

// Define Output //  
Atlas AtlasPopulationRegistration::defineRegisteredAtlas(Atlas atlas)
{
   Atlas atlasReg = atlas;

   QString atlas_path = m_module_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path); 

   if(!(atlas.T1).isEmpty())
   {
      QString T1Reg_name = atlas.name + "_to_" + m_neo.prefix + "-T1.nrrd";
      atlasReg.T1 = atlas_dir->filePath(T1Reg_name);
   } 
   else
   {
      atlasReg.T1 = "";
   }

   if(!(atlas.T2).isEmpty())
   {
      QString T2Reg_name = atlas.name + "_to_" + m_neo.prefix + "-T2.nrrd";
      atlasReg.T2 = atlas_dir->filePath(T2Reg_name);
   }
   else
   {
      atlasReg.T2 = ""; 
   }

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

std::vector<Atlas> AtlasPopulationRegistration::defineRegisteredAtlasPopulation()
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
void AtlasPopulationRegistration::createAtlasDirectories()
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

void AtlasPopulationRegistration::initializeScript()
{
   definePython();
   importGeneralModules();
  
   m_script += "import time\n";
   m_script += "import array\n";
   m_script += "import re\n";
   m_script += "from collections import namedtuple\n\n";

   m_script += "logger = logging.getLogger('NeosegPipeline')\n\n";

   m_script += "runningRegistrations = None\n\n";  
}

void AtlasPopulationRegistration::defineRegisterAtlasParameters(Atlas atlas)
{
   m_script += "\t# Registration " + atlas.name + " to " + m_neo.prefix + "\n"; 

   m_script += "\tname = '" + atlas.name + "'\n"; 

   m_script += "\tT1Atlas = '" + atlas.T1 + "'\n";
   m_script += "\tT2Atlas = '" + atlas.T2 + "'\n";

   if(atlas.probabilistic)
   {
      m_script += "\twhiteAtlas = '" + atlas.white + "'\n";
      m_script += "\tgrayAtlas = '" + atlas.gray + "'\n";
      m_script += "\tcsfAtlas = '" + atlas.csf + "'\n";
   }
   
   else
   {
      m_script += "\tsegAtlas = '" + atlas.seg + "'\n";
   }

   QString atlas_path = m_module_dir->filePath(atlas.name);
   QDir* atlas_dir = new QDir(atlas_path);

   QString output_path = atlas_dir->absolutePath();
   m_script += "\toutput = '" + output_path + "'\n";

   QString basename = atlas.name + "_to_" + m_neo.prefix;

   QString log_name = basename + ".log";     
   QString log = atlas_dir->filePath(log_name);
   m_script += "\tlog = '" + log + "'\n\n";
   
   QString affine_path = atlas_dir->filePath(basename + "_Affine.txt");  
   QString affineTemp_path = atlas_dir->filePath(basename + "_Affine.txt~");  
   QString warp_path = atlas_dir->filePath(basename + "_Warp.nii.gz"); 
   QString inverseWarp_path = atlas_dir->filePath(basename + "_InverseWarp.nii.gz"); 

   // for keven lee cleaning
   QString smoothedMask_path = atlas_dir->filePath(m_prefix + "mask-smoothed" + m_suffix + ".nrrd"); 

   m_unnecessaryFiles << affine_path;
   m_unnecessaryFiles << affineTemp_path; 
   m_unnecessaryFiles << warp_path; 
   m_unnecessaryFiles << inverseWarp_path; 
   m_unnecessaryFiles << smoothedMask_path; 
}

void AtlasPopulationRegistration::submitRegisterAtlasJob(Atlas atlas, int i)
{
   QString QString_i; 
   QString_i.setNum(i);

   QString process_name = "process_" + QString_i;
   QString registration_name = "registration_" + QString_i;

   QString args = "'bsub', '-q', 'day', '-M', '" + QString::number(m_parameters->getNumberOfGB()) + "', '-n', '" + QString::number(m_parameters->getNumberOfCores()) + "', '-R', 'span[hosts=1]', ";

   if(atlas.probabilistic)
   {
      args += "'python', probabilisticAtlasRegistration_script, name, T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, output, log";
   }
   
   else
   {
      args += "'python', atlasRegistration_script, name, T1Atlas, T2Atlas, segAtlas, output, log";
   }
   m_script += "\targs = [" + args + "]\n";
   m_script += "\tbsub_process = subprocess.Popen(args, stdout=subprocess.PIPE)\n";
   m_script += "\tbsub_output = bsub_process.stdout.read()\n";
   m_script += "\tlogger.info(bsub_output)\n";
   m_script += "\tjobID = re.search('(<{1})([0-9]{1,})(>{1})', bsub_output).group(2)\n"; 
   m_script += "\tlogger.info(jobID)\n"; 
   m_script += "\t" + registration_name + " = registration('" + atlas.name + "', jobID)\n";
   m_script += "\trunningRegistrations.append(" + registration_name + ")\n\n";

}

void AtlasPopulationRegistration::executeRegisterAtlasProcess(Atlas atlas, int i) 
{
   QString QString_i; 
   QString_i.setNum(i);

   if(m_parameters->getNumberOfRegistrations() != 0)
   {
      m_script += "\ttime.sleep(1)\n";
      m_script += "\twhile len(runningRegistrations) >= nbRegMax :\n";  
      m_script += "\t\tcheckRunningRegistrations(runningRegistrations)\n";
      m_script += "\t\ttime.sleep(1)\n";
   }

   QString process_name = "process_" + QString_i;
   QString registration_name = "registration_" + QString_i;

   QString command;

   if(atlas.probabilistic)
   {
      command = "['python', probabilisticAtlasRegistration_script, name, T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, output, log]";
   }
   else
   {
      command = "['python', atlasRegistration_script, name, T1Atlas, T2Atlas, segAtlas, output, log]";
   }
   
   
   m_script += "\t" + process_name + " = subprocess.Popen(" + command + ")\n";

   m_script += "\t" + registration_name + " = registration('" + atlas.name + "', " + process_name + ")\n";
   m_script += "\trunningRegistrations.append(" + registration_name + ")\n\n";
}


void AtlasPopulationRegistration::implementRegisterAtlasPopulation()
{
   int i;

   if(m_computingSystem == "local")
   {
      m_script += "def stop(signal, frame):\n";
      m_script += "\tprint 'Signal received'\n";
      m_script += "\tfor registration in runningRegistrations:\n";
      m_script += "\t\t(registration.process).terminate()\n";
      m_script += "\tsys.exit(0)\n\n";

      m_script += "def checkRunningRegistrations(runningRegistrations):\n";   
      m_script += "\tfor registration in runningRegistrations :\n";
      m_script += "\t\tif (registration.process).poll()==0:\n";
      m_script += "\t\t\trunningRegistrations.remove(registration)\n\n";
   }

   if(m_computingSystem == "killdevil" || m_computingSystem == "killdevil interactive")
   {
      m_script += "def stop(signal, frame):\n";
      m_script += "\tprint 'Signal received'\n";
      m_script += "\tfor registration in runningRegistrations:\n";
      m_script += "\t\tbkill_process = subprocess.Popen(['bkill', registration.job])\n";
      m_script += "\tsys.exit(0)\n\n";

      m_script += "def checkRunningRegistrations():\n";   
      m_script += "\tfor registration in runningRegistrations :\n";
      m_script += "\t\tbjobs_process = subprocess.Popen(['bjobs', registration.job], stdout=subprocess.PIPE)\n";
      m_script += "\t\tbjobs_output = bjobs_process.stdout.read()\n";
      m_script += "\t\tif 'DONE' in bjobs_output or 'EXIT' in bjobs_output :\n";
      m_script += "\t\t\trunningRegistrations.remove(registration)\n\n";
   }

   m_script += "def run():\n";

   m_script += "\tlogger.info('=== Atlas Registration ===')\n";

   m_script += "\tsignal.signal(signal.SIGINT, stop)\n";
   m_script += "\tsignal.signal(signal.SIGTERM, stop)\n\n";

   if(m_parameters->getNumberOfCores() != 0)
   {
      m_script += "\tos.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS'] = '" + QString::number(m_parameters->getNumberOfCores()) + "' \n";
   }
   if(m_parameters->getNumberOfRegistrations() != 0)
   {
      m_script += "\tnbRegMax = " + QString::number(m_parameters->getNumberOfRegistrations()) +  "\n";
   }

   m_script += "\trunningRegistrations = []\n\n";
   m_script += "\tglobal runningRegistrations\n";

   if(m_computingSystem == "killdevil" || m_computingSystem == "killdevil interactive")
   {
      m_script += "\tregistration = namedtuple('registration', 'name job')\n\n";
   }
   else
   {
      m_script += "\tregistration = namedtuple('registration', 'name process')\n\n";
   }

   QString atlasRegistration_script = m_processing_dir->filePath("atlasRegistration.py");
   m_script += "\tatlasRegistration_script = '" + atlasRegistration_script + "'\n";

   QString probabilisticAtlasRegistration_script = m_processing_dir->filePath("probabilisticAtlasRegistration.py");
   m_script += "\tprobabilisticAtlasRegistration_script = '" + probabilisticAtlasRegistration_script + "'\n\n";

   std::vector<Atlas>::iterator it;
   for (it = m_atlasPopulation.begin(); it != m_atlasPopulation.end(); ++it)
   {  
      i = std::distance(m_atlasPopulation.begin(), it);

      Atlas atlas = *(it);

      defineRegisterAtlasParameters(atlas);

      if(m_computingSystem == "killdevil" || m_computingSystem == "killdevil interactive")
      {
         submitRegisterAtlasJob(*it, i);
      }

      if (m_computingSystem == "local")
      {
         executeRegisterAtlasProcess(*it, i);
      }
   }

   if(m_computingSystem == "local")
   {
      m_script += "\texit_codes = [(registration.process).wait() for registration in runningRegistrations]\n";
   }

   if(m_computingSystem == "killdevil" || m_computingSystem == "killdevil interactive")
   {
      m_script += "\twhile len(runningRegistrations) !=0 :\n";
      m_script += "\t\tcheckRunningRegistrations()\n"; 
      m_script += "\t\ttime.sleep(1)\n";
   }

   m_script += "\tlogger.info('')\n"; 
}

void AtlasPopulationRegistration::writeRegisterAtlasPopulation()
{
   initializeScript();
   implementCheckFileExistence(); 
   implementRegisterAtlasPopulation();
   writeScript();
}

void AtlasPopulationRegistration::update()
{   
   defineRegisteredAtlasPopulation();
   createAtlasDirectories();
   writeRegisterAtlasPopulation();
}

std::vector<Atlas> AtlasPopulationRegistration::getOutput()
{
   return defineRegisteredAtlasPopulation();
}

