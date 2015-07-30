// Generate CLP
#include "WeightedLabelsAverageCLP.h"

// General libraries // 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <stdio.h>

// ITK librairies //
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkWeightedLabelsAverageFilter.h"
#include "itkXMLFile.h"

// XML Librairies // 
#include "Parameters.h"
#include "ParametersXMLFile.h"

#include "Atlas.h"

int main(int argc, char* argv[])
{

   PARSE_ARGS;

   // Types // 
   typedef itk::Image<float, 3>                    FloatImageType;
   typedef itk::ImageFileReader<FloatImageType>    ReaderType; 
   typedef itk::ImageFileWriter<FloatImageType>    WriterType; 

   ParametersXMLFileReader::Pointer reader = ParametersXMLFileReader::New();
   reader->SetFilename(parameters_file);
   reader->GenerateOutputInformation();
   Parameters::Pointer parameters = reader->GetOutputObject();

   if (!parameters->CheckValues())
   {
      std::cout<<"Invalid Parameters"<<std::endl; 
   }

   // Tissue Probability Maps Filter 
   typedef itk::WeightedLabelsAverageFilter< FloatImageType, FloatImageType > WeightedLabelsAverageFilterType;
   WeightedLabelsAverageFilterType::Pointer WeightedLabelsAverageFilter = WeightedLabelsAverageFilterType::New();

   // Weights 
   if(parameters->GetComputingWeights())
   {
      WeightedLabelsAverageFilter->SetComputingWeightsOn();
      WeightedLabelsAverageFilter->SetRadiusValue(parameters->GetNeightborhoodRadius());
      if(parameters->GetNeightborhoodRadiusUnit()=="mm")
      {
         WeightedLabelsAverageFilter->SetRadiusValueInMillimetersOn();
      }
      else
      {
         WeightedLabelsAverageFilter->SetRadiusValueInMillimetersOff();         
      }
   }

   else
   {
      WeightedLabelsAverageFilter->SetComputingWeightsOff();
      //WeightedLabelsAverageFilter->SetNumberOfThreads(2);
   }


   std::string inputFilename = parameters->GetInput();
   std::string inputExtension = inputFilename.substr(inputFilename.find_last_of("."));
   if(inputExtension.compare(".gz") == 0){
    inputExtension = ".nii.gz";
   }
   // Input
   ReaderType::Pointer input_reader = ReaderType::New();
   input_reader->SetFileName(parameters->GetInput());
   input_reader->Update();
   WeightedLabelsAverageFilter->SetInput(input_reader->GetOutput());

   // Atlas Population
   std::vector<Atlas> atlasPopulation = parameters->GetAtlasPopulation();
   Atlas atlas;
   std::vector<Atlas>::iterator it;
   for( it=atlasPopulation.begin(); it!=atlasPopulation.end(); ++it)
   {
       Atlas atlas = *(it);

       // Image
       ReaderType::Pointer image_reader = ReaderType::New();
       image_reader->SetFileName(atlas.image);
       image_reader->Update();

       if(atlas.probabilistic)
       {
           ReaderType::Pointer white_reader = ReaderType::New();
           white_reader->SetFileName(atlas.white);
           white_reader->Update();

           ReaderType::Pointer gray_reader = ReaderType::New();
           gray_reader->SetFileName(atlas.gray);
           gray_reader->Update();

           ReaderType::Pointer csf_reader = ReaderType::New();
           csf_reader->SetFileName(atlas.csf);
           csf_reader->Update();

           WeightedLabelsAverageFilter->AddAtlas(image_reader->GetOutput(), white_reader->GetOutput(), gray_reader->GetOutput(), csf_reader->GetOutput());
       }
       else
       {
           ReaderType::Pointer seg_reader = ReaderType::New();
           seg_reader->SetFileName(atlas.seg);
           seg_reader->Update();
           WeightedLabelsAverageFilter->AddAtlas(image_reader->GetOutput(), seg_reader->GetOutput());
       }
   }

   // Update 
   WeightedLabelsAverageFilter->Update();


   std::cout<<"Writing Outputs: "<<std::endl;
   for(unsigned i = 0; i < WeightedLabelsAverageFilter->GetNumberOfOutputs(); i++){
    WriterType::Pointer writer_white = WriterType::New(); 
     writer_white->SetInput(WeightedLabelsAverageFilter->GetOutput(i)); 
     std::string outfilename = std::to_string(i + 1) + inputExtension;
     std::cout<<"\t"<<outfilename<<std::endl;
     writer_white->SetFileName(outfilename.c_str());
     writer_white->SetUseCompression(true);
     writer_white->Update();
   }


   // // Weighted Averaged White // 
   // WriterType::Pointer writer_white = WriterType::New(); 
   // writer_white->SetInput(WeightedLabelsAverageFilter->GetOutput(0)); 
   // writer_white->SetFileName(parameters->GetWhite());
   // writer_white->SetUseCompression(true);
   // writer_white->Update();
   
   // // Weighted Averaged Gray // 
   // WriterType::Pointer writer_gray = WriterType::New(); 
   // writer_gray->SetInput(WeightedLabelsAverageFilter->GetOutput(1)); 
   // writer_gray->SetFileName(parameters->GetGray());
   // writer_gray->SetUseCompression(true);
   // writer_gray->Update();
   
   // // Weighted Averaged Csf // 
   // WriterType::Pointer writer_csf = WriterType::New(); 
   // writer_csf->SetInput(WeightedLabelsAverageFilter->GetOutput(2)); 
   // writer_csf->SetFileName(parameters->GetCsf());
   // writer_csf->SetUseCompression(true);
   // writer_csf->Update();
}


