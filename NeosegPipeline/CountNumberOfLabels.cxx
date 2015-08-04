
#include "CountNumberOfLabels.h"

using namespace std;

CountNumberOfLabels::CountNumberOfLabels(){
    m_NumberOfLabels = 0;
    m_Filename = "";
}

void CountNumberOfLabels::Update()
{

    if(m_Filename.compare("") == 0){
        cout<<"Set filename of label map to count labels. "<<endl;
        return;
    }

  InputImageReaderPointerType reader = InputImageReaderType::New();

  reader->SetFileName(m_Filename);
  reader->Update();
  InputImagePointerType inputImage = reader->GetOutput();

  m_ImageLabelMap.clear();

  InputImageIteratorType it(inputImage, inputImage->GetLargestPossibleRegion());
  it.GoToBegin();

  int labelIndex = 0;

  while(!it.IsAtEnd()){
    InputPixelType p = it.Get();
      
    if(m_ImageLabelMap.find(p) == m_ImageLabelMap.end()){
      m_ImageLabelMap[p] = labelIndex;
      labelIndex++;
    }
        
    ++it;
  }


  InputImageLabelMapIteratorType itmap;
  labelIndex = 0;
  for(itmap = m_ImageLabelMap.begin(); itmap != m_ImageLabelMap.end(); ++itmap){
      m_ImageLabelMap[itmap->first] = labelIndex;
      labelIndex++;
  }

  m_NumberOfLabels = m_ImageLabelMap.size();

}
