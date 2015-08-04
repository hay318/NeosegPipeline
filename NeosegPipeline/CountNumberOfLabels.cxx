
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

  InputImageLabelMap inputLabelsMap;

  InputImageIteratorType it(inputImage, inputImage->GetLargestPossibleRegion());
  it.GoToBegin();

  int labelIndex = 0;

  while(!it.IsAtEnd()){
    InputPixelType p = it.Get();
      
    if(inputLabelsMap.find(p) == inputLabelsMap.end()){
      inputLabelsMap[p] = labelIndex;
      labelIndex++;
    }
        
    ++it;
  }

  m_NumberOfLabels = inputLabelsMap.size();

}
