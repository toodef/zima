#include <iostream>

#include "gdal_priv.h"
#include "cpl_conv.h"


int main()
{
   GDALDataset *poDataset;
   GDALAllRegister();
   poDataset = (GDALDataset *) GDALOpen("a.tif", GA_ReadOnly);
   if (poDataset == NULL) {
      std::cout << "Nakonetcta!!!" << std::endl;
   }
}
//#include <iostream>
//
////#include <QtWidgets/QtWidgets>
//
//int main()
//{
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
//}