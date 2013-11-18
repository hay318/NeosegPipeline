#include "Pipeline.h"


Pipeline::Pipeline () : m_preProcessingData(NULL), m_atlasRegistration(NULL) //, m_atlasGeneration_script(NULL), m_neosegExecution_script(NULL)
{
   // Test //

   m_output_path="/home/cherel/neosegPipeline/test";
   m_neo.T1_path="/home/cherel/neosegPipeline/data/neo-0106-1-T1.gipl.gz";
   m_neo.T2_path="/home/cherel/neosegPipeline/data/neo-0106-1-T2.gipl.gz";
   m_mask_path="/home/cherel/neosegPipeline/data/neo-0106-1-T2_seg.gipl.gz";
   m_skullStripping=false;
}

QFileInfoList Pipeline::find(QDir* dir, QString name)
{
   name = "*" + name + "*";
   QStringList* filter = new QStringList(name);
   QFileInfoList foundFile_list = dir->entryInfoList(*filter);

   return foundFile_list;
} 

void Pipeline::setT1 (QString T1_path) {m_neo.T1_path=T1_path;}

void Pipeline::setT2 (QString T2_path) {m_neo.T2_path=T2_path;}

void Pipeline::setOutput (QString output_path) {m_output_path = output_path;}

void Pipeline::setMask (bool mask) {m_mask=mask;}

void Pipeline::setMask (QString mask) {m_mask_path=mask;}

void Pipeline::setSkullStripping (bool skullStripping) {m_skullStripping=skullStripping;}

void Pipeline::setOldAtlas(QString oldAtlas) {m_oldAtlas=oldAtlas;}

void Pipeline::setAtlasPopulation(QString atlasPopulation_path, QStringList selectedAtlases) 
{
   QStringList::const_iterator it;

   for (it = selectedAtlases.constBegin(); it != selectedAtlases.constEnd(); ++it) 
   {
      Atlas atlas;

      // Create the directory 
      QString atlas_path = atlasPopulation_path + "/" + *it;
      QDir* atlas_dir = new QDir(atlas_path);
      atlas.name=*it;

      // T1 and T2
      QFileInfoList T1 = find(atlas_dir, "T1");
      QFileInfoList T2 = find(atlas_dir, "T2");

      atlas.T1=T1[0].filePath();
      atlas.T2=T2[0].filePath();

      // Hard seg
      QFileInfoList seg = find(atlas_dir, "seg");

      if (seg.size()==1)
      {
         atlas.seg=seg[0].filePath();
      }

      // Soft seg 
      QFileInfoList white = find(atlas_dir, "white");
      QFileInfoList gray = find(atlas_dir, "gray");
      QFileInfoList csf = find(atlas_dir, "csf");

      if ((white.size()==1) && (gray.size()==1) && (csf.size()==1))
      {
         atlas.white=white[0].filePath();
         atlas.gray=gray[0].filePath();
         atlas.csf=csf[0].filePath();
      }

      m_atlasPopulation.push_back(atlas);
   } 
}

bool Pipeline::checkAtlas(QString atlas_path)
{
   QDir* atlas_dir = new QDir(atlas_path);

   QFileInfoList templateT1 = find(atlas_dir, "templateT1");
   QFileInfoList templateT2 = find(atlas_dir, "templateT2");

   QFileInfoList white = find(atlas_dir, "white");
   QFileInfoList gray = find(atlas_dir, "gray");
   QFileInfoList csf = find(atlas_dir, "csf");
   QFileInfoList rest = find(atlas_dir, "rest");

   if ((templateT1.size()==1) && (templateT2.size()==1) && (white.size()==1) && (gray.size()==1) && (csf.size()==1) && (rest.size())) {return true;}

   else {return false;}
}

bool Pipeline::checkAtlasPopulation(QString atlas_path)
{ 
   QDir* atlas_dir = new QDir(atlas_path);

   QFileInfoList T1 = find(atlas_dir, "T1");
   QFileInfoList T2 = find(atlas_dir, "T2");

   // Hard segmentation
   QFileInfoList seg = find(atlas_dir, "seg");
   if ((T1.size()==1) && (T2.size()==1) && (seg.size()==1)){return true;}

   // Soft segmentation
   QFileInfoList white = find(atlas_dir, "white");
   QFileInfoList gray = find(atlas_dir, "gray");
   QFileInfoList csf = find(atlas_dir, "csf");
   if (((T1.size()==1) && (T2.size()==1) && (white.size()==1) && (gray.size()==1) && (csf.size()==1))) {return true;}

   else {return false;}
}

void Pipeline::setSmoothing (QString smoothing) {m_smoothing = smoothing;}

void Pipeline::setSmoothingSize (double smoothingSize) {m_smoothingSize = smoothingSize;}

void Pipeline::setOverwriting (bool overwriting) {m_overwriting = overwriting;}

void Pipeline::setCleaningUp (bool cleaningUp) {m_cleaningUp = cleaningUp;}

void Pipeline::setComputingSystem (QString computingSystem) {m_computingSystem = computingSystem;}

void Pipeline::setNbCores (int nbCores) {m_nbCores = nbCores;}


void Pipeline::runPreProcessingData()
{
   PreProcessingData* m_preProcessingData= new PreProcessingData(m_output_path);
   
   if(!m_preProcessingData->checkExistingResults(m_neo))
   {
      m_preProcessingData->createDirectory();
      m_preProcessingData->createScript(); 

      if(!m_mask)
      {
         m_mask_path = m_preProcessingData->runNeoseg(m_neo.T2_path);                   
      }

      m_neo = m_preProcessingData->skullStripNeo(m_neo, m_mask_path);
   }
}


void Pipeline::runAtlasRegistration()
{
   AtlasRegistration* m_atlasRegistration= new AtlasRegistration(m_output_path, m_computingSystem);
  
   if(!m_atlasRegistration->checkExistingResults(m_atlasPopulation))
   {
      m_atlasRegistration->createDirectory();
      m_atlasRegistration->createAtlasDirectories(m_atlasPopulation);

      m_atlasRegistration->writeRegisterAtlas(m_neo);
      m_atlasRegistration->writeRegisterAtlasPopulation(m_atlasPopulation);

      m_atlasRegistration->updateAtlasPopulation(m_atlasPopulation);
   }
}

void Pipeline::runAtlasGeneration()
{
   // Create Atlas Generation script
   //AtlasGeneration AtlasGeneration_script(output_Dir,"AtlasGeneration");
}

void Pipeline::runNeosegExexution()
{
   // Create Neoseg Execution script
   //NeosegExecution NeosegExecution_script(output_Dir, "NeosegExecution");
}


void Pipeline::runPipeline()
{
   m_script = "#!/usr/bin/python3.3 \n\n";
   m_script += "import os \n";
   m_script += "import sys \n\n";

   if (m_skullStripping==false)
   { 
      //runPreProcessingData();
   }

   if (m_newAtlas==true)
   {
      runAtlasRegistration();
      
      //runAtlasGeneration();
   }

   //runNeosegExecution


   //write main script (Function)
   QDir* output_dir = new QDir(m_output_path); 
   QString script_path = output_dir->filePath("main.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);

   *script_stream << m_script.toStdString() << std::endl; 

   //run main script  
}

