#ifndef COUNTNUMBEROFLABELS_H
#define COUNTNUMBEROFLABELS_H

#include "itkImageFileReader.h"
#include "itkImageRegionIterator.h"

using namespace std;

class CountNumberOfLabels{
public:
    CountNumberOfLabels();

    static const int dimension = 3;

    typedef double InputPixelType;
    typedef itk::Image< InputPixelType, dimension > InputImageType;
    typedef InputImageType::Pointer InputImagePointerType;

    typedef itk::ImageFileReader< InputImageType > InputImageReaderType;
    typedef InputImageReaderType::Pointer InputImageReaderPointerType;

    typedef itk::ImageRegionIterator<InputImageType> InputImageIteratorType;

    typedef map< InputPixelType, int > InputImageLabelMapType;
    typedef InputImageLabelMapType::iterator InputImageLabelMapIteratorType;

    int GetNumberOfLabels(){
        return m_NumberOfLabels;
    }

    void SetFilename(string filename){
        m_Filename = filename;
    }

    InputImageLabelMapType GetImageLabelMap(){
        return m_ImageLabelMap;
    }

    void Update();

private:

    string m_Filename;
    int m_NumberOfLabels;
    InputImageLabelMapType m_ImageLabelMap;


};

#endif // COUNTNUMBEROFLABELS_H

