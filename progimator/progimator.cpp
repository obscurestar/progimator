#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#include "animation.hpp"

std::vector<Animation> readFile(std::string filename)
{
    std::ifstream infile(filename);
    std::string line;

    std::vector<Animation> anims;
    if (!infile.is_open())
    {
        std::cerr << "Unable to open file " << filename << std::endl;
        return anims;
    }
    
    while (std::getline(infile, line))
    {
        anims.push_back( Animation(line) );
    }
    infile.close();

    return anims;
}

/*
 const short ANIM_NUM PROGMEM = 123;
 const short ANIM_DIM[2] PROGMEM = { x, y };
 const short ANIM_ROWLEN PROGMEM = 12;
 const short ANIM_FRAMESIZE PROGMEM = x*y;
 const short ANIM_FRAME_COUNTS[ANIM_NUM] PROGMEM = { 9, 3, 7 };
 const short ANIM_FRAME_INDEX[ANIM_NUM] PROGMEM = {0, 9, 12 };
 const unsigned char ANIM_FRAME_DATA[TOTAL_FRAMES][ANIM_FRAMESIZE] PROGMEM = {
  { 0x0A, 0x0F, .. }  //One for each frame.
 };
 */

void generate_anim_header( std::string filename, std::vector<Animation> anims )
{
    if (anims.empty())
    {
        std::cout << "No data to write." << std::endl;
        return;  //No data.
    }
    
    std::ofstream outfile;

    outfile.open(filename, std::ios::out);

    if (!outfile.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    outfile << "#ifndef anim_h" << std::endl;
    outfile << "#define anim_h" << std::endl;
    
    Animation anim = anims[0];
    
    outfile << "const short ANIM_NUM PROGMEM = " << anims.size() << ";" << std::endl;
    outfile << "const short ANIM_DIR[2] PROGMEM = " << anim.getDimString() << ";" << std::endl;
    outfile << "const short ANIM_ROWLEN PROGMEM = " << anim.getRowLength() << ";" << std::endl;
    outfile << "const short ANIM_FRAMEBYTES PROGMEM = " << anim.getFrameBytes() << ";" << std::endl;
    
    outfile << "const short ANIM_FRAME_COUNTS[" << anims.size() << "] PROGMEM = { ";
    
    std::ostringstream offsets;
    
    int last_comma = (int)anims.size() - 1;
    int offset = 0;
    for (int i=0;i<anims.size();++i)
    {
        Animation a=anims[i];
        outfile << a.getNumFrames();
        offsets << offset;
        offset += a.getNumFrames();
        
        if(i < last_comma)
        {
            outfile << ", ";
            offsets << ", ";
        }
    }
    outfile << " };" << std::endl;
    
    outfile << "const short ANIM_FRAME_INDEX[" << anims.size() << "] PROGMEM = { "
        << offsets.str() << " };" << std::endl;
    
    outfile << "const short ANIM_FRAME_MS[" << anims.size() << "] PROGMEM = { ";
    for (int i=0;i<anims.size();++i)
    {
        Animation a = anims[i];
        outfile << a.getFrameMS();
        
        if (i < last_comma)
        {
            outfile << ", ";
        }
    }
    outfile << " };" << std::endl;
    
    outfile << "const unsigned char ANIM_FRAME_DATA[" << offset << "]["
        << anim.getFrameBytes() << "] PROGMEM = {";
    
    for (int i=0;i<anims.size();++i)
    {
        Animation a = anims[i];
        int lc2 = a.getNumFrames() - 1;
        for (int j=0;j<a.getNumFrames();++j)
        {
            outfile << a.getFrameString(j);
            if (j < lc2)
            {
                outfile << ",";
            }
            outfile << std::endl;
        }
        if (i < last_comma)
        {
            outfile << ",";
        }
        outfile << std::endl;
    }
    outfile << "};" << std::endl;

    outfile << "#endif /*anim_h*/" << std::endl;
    outfile.close();
}

int main(int argc, const char * argv[]) {
    
    if (argc != 3)
    {
        std::cout << "Syntax: " << argv[0] << " <INPUT_ANIMATION_FILE> <OUPUT_HEADER_FILE>" << std::endl;
        return 0;
    }
    
    std::string inname = argv[1];
    std::string outname =argv[2];
    
    std::cout << "Got filename: " << inname << std::endl;
    
    std::vector<Animation> anims = readFile(inname);
    
    generate_anim_header(outname, anims);
    
    return 0;
}
