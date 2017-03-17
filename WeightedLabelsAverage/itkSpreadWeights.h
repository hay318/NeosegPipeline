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
               OutputPixelType result = 0.5+(erf((x-0.95)*40)+1)/4;
	       // 0.9 => -2 /-1
	       // 1.0 => +2 / 1
	       // half weight at 0.95 +/- 0.0125
               return result;
            }
      };
   //}
}

#endif
