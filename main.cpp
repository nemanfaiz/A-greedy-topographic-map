#include "Topograph.h"


int main ()
{
    //constructing a Topograph object with follwoing name  
    string objName = "map_input.txt";
    Topograph obj (objName);

    //constructing bmp object with Topograph's height and width 
    Bmp bmpObj(obj.getHeight(), obj.getWidth());
    //calling my drawMap and passsing my bmp object into it
    obj.drawMap(bmpObj);

    //writing on bmp object with follownig name
    string bmpName = "map.bmp";
    bmpObj.write(bmpName);

    //using system to view the map.bmp image
    system(("eog " + bmpName + " &").c_str()); ///Linux system call to open the image file for viewing

    //calling my drawBestPath and apssing in my bmp object into it
    obj.drawBestPath(bmpObj);
    
    //writing on bmp object with following name
    string bmpName2 = "map2.bmp";
    bmpObj.write(bmpName2);

    //using system to view the map2.bmp image
    system(("eog " + bmpName2 + " &"). c_str());
}