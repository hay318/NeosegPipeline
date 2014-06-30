// Generate CLP // 
#include <ReassignWhiteMatterCLP.h>

// General libraries // 
#include <iostream>
#include <string>

// ITK librairies //
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkBinaryImageToLabelMapFilter.h"
#include "itkBinaryThresholdImageFilter.h"

#include "itkXMLFile.h"

// XML Librairies // 
#include "Parameters.h"
#include "ParametersXMLFile.h"

int main(int argc, char* argv[])
{
   PARSE_ARGS;

   ParametersXMLFileReader::Pointer reader_XML = ParametersXMLFileReader::New();
   reader_XML->SetFilename(parameters_file);
   reader_XML->GenerateOutputInformation();
   Parameters::Pointer parameters = reader_XML->GetOutputObject();

   if (!parameters->CheckValues())
   {
      std::cout<<"Invalid Parameters"<<std::endl; 
   }

   // Image Type
   typedef itk::Image< float, 3>    ImageType;
   typedef ImageType::IndexType                 ImageIndexType;
   typedef ImageType::PixelType                 ImagePixelType;
   typedef ImageType::Pointer                   ImagePointerType;

   typedef itk::ImageFileReader<ImageType> ReaderType; 
   typedef itk::ImageFileWriter<ImageType> WriterType; 
   
   // Input Reader
	ReaderType::Pointer reader_input = ReaderType::New();
	reader_input->SetFileName(parameters->GetInput());
   reader_input->Update();
   ImagePointerType input = reader_input->GetOutput();

   // White Reader 
	ReaderType::Pointer reader_white = ReaderType::New();
	reader_white->SetFileName(parameters->GetWhite());
   reader_white->Update();
   ImagePointerType white = reader_white->GetOutput();

   // Gray Reader 
	ReaderType::Pointer reader_gray = ReaderType::New();
	reader_gray->SetFileName(parameters->GetGray());
   reader_gray->Update();
   ImagePointerType gray = reader_gray->GetOutput();

   // Caf Reader 
	ReaderType::Pointer reader_csf = ReaderType::New();
	reader_csf->SetFileName(parameters->GetCsf());
   reader_csf->Update();
   ImagePointerType csf = reader_csf->GetOutput();

   // Input Reader
   ImagePointerType output = input;

   // Label Map Filters 
   typedef itk::BinaryImageToLabelMapFilter<ImageType> BinaryImageToLabelMapFilterType;
   BinaryImageToLabelMapFilterType::Pointer labelMapFilters[3];

   // Number of Connected Components
   int numberOfConnectedComponents[3];

   for(int label = 0; label < 3; label++)
   {
      typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;
      BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
      thresholdFilter->SetInput(reader_input->GetOutput());
      thresholdFilter->SetLowerThreshold(label+1);
      thresholdFilter->SetUpperThreshold(label+1);
      thresholdFilter->SetInsideValue(255);
      thresholdFilter->SetOutsideValue(0);

      BinaryImageToLabelMapFilterType::Pointer labelMapFilter = BinaryImageToLabelMapFilterType::New();
      labelMapFilter->SetInput(thresholdFilter->GetOutput());
      labelMapFilter->SetOutputBackgroundValue(0);
      labelMapFilter->SetInputForegroundValue(255);
      labelMapFilter->SetFullyConnected(true);
      labelMapFilter->Update();

      labelMapFilters[label] = labelMapFilter;
      numberOfConnectedComponents[label] = (labelMapFilter->GetOutput())->GetNumberOfLabelObjects();
   }

   // Connected Component Size
   int connectedComponentSize[3];

   // Max Number of Connected Components
   int max = *std::max_element(numberOfConnectedComponents, numberOfConnectedComponents + 3);

   for(int i = 1; i <= max; i++)
   {
      for(int label = 0; label < 3; label++)
      {
         if(i < numberOfConnectedComponents[label])
         {
            connectedComponentSize[label] = ((labelMapFilters[label]->GetOutput())->GetLabelObject(i))->Size(); 
         }
         else
         {
            connectedComponentSize[label] = 0;
         }
      }

      if(connectedComponentSize[0] < parameters->GetThreshold())
      {
         for(int voxel = 0; voxel < connectedComponentSize[0]; voxel++)
         {
            ImageIndexType index; 

            index = ((labelMapFilters[0]->GetOutput())->GetLabelObject(i))->GetIndex(voxel);

            ImagePixelType probability_gray = gray->GetPixel(index); 
            ImagePixelType probability_csf = csf->GetPixel(index); 

            if(probability_gray > probability_csf)
            {
               output->SetPixel(index, 2); 
            }
            else
            {
               output->SetPixel(index, 3);
            }
         }       
      }
   }

   // Output Writer
	WriterType::Pointer writer_output = WriterType::New();
	writer_output->SetFileName(parameters->GetOutput());
	writer_output->SetInput(output);
   writer_output->Update();

   return 0; 
}
