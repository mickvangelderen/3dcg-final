/* loadppm.cpp */
#include "loadppm.h"

PPMImage::PPMImage(const char *filename)
{
    char buff[16];

    ifstream fp(filename, ios::in|ios::binary);
    int maxval,lig_comm=0;

    if (!fp.is_open()){
	cerr <<"Unable to open file " << filename << endl;
	throw 0;
    }

    fp.read(buff, 3);
    if (fp.bad()){
      cerr <<"Unable to open file " << filename << endl;
      throw 0;
    }
    
    if (buff[0] != 'P' || buff[1] != '6'){
	cerr << "Invalid image format (must be `P6')\n" ;
	throw 0;
    }

    if (buff[2] != 0x0a)
      fp.seekg(-1,ios::cur);

    while (fp.read(buff, 1), buff[0] == '#'){
      lig_comm++;
      while (fp.read(buff, 1), buff[0] != '\n');
    }

    fp.seekg(-1,ios::cur);

    fp >> sizeX;
    fp >> sizeY;
    if (fp.bad()){
      cerr << "Error loading image " << filename << endl ;
      throw 0;
    }

    fp >> maxval;
    if (fp.bad()){
      cerr << "Error loading image " << filename << endl ;
      throw 0;
    }

    while (fp.read(buff, 1), buff[0] != '\n')
      ;

    data = new  GLubyte[3 * sizeX * sizeY];
    if (!data)
    {
      cerr <<  "Unable to allocate memory\n";
      throw 0;
    }

    // Chargement de l'image directement
    fp.read((char*)data, 3 * sizeX*sizeY);
    if (fp.bad()) {
      cerr <<  "Error loading image " << filename << endl;
      throw 0;
    }
    
    fp.close();
}

