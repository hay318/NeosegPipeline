#ifndef __itkSpreadWeights_h
#define __itkSpreadWeights_h

namespace itk
{
   //namespace Functor
   //{
      template< class InputPixelType, class OutputPixelType >
      class SpreadWeights
      {
         public:
            SpreadWeights() {}
            ~SpreadWeights() {}
 
            bool operator!=(const SpreadWeights & other) const
            {
               return false;
            }
 
            bool operator==(const SpreadWeights & other) const
            {
               return !( *this != other );
            }
 
            inline OutputPixelType operator()(const InputPixelType & x) const
            { 
               OutputPixelType result = (erf((x-0.95)*60)+1)/2;
               return result;
            }
      };
   //}
}

#endif
