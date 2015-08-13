#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "lodepng.h"
using namespace std;

int main(int argc, char *args[])
{
	if (argc <= 1)
	{
		cout << "Must supply a file for visualization." << endl;
		return -1;
	}

	// Open File
		auto t_FileName = string(args[1]);
		ifstream t_Input(t_FileName, ios::binary);

	if (t_Input.is_open())
	{
		// Load File into Buffer
			vector<unsigned char> t_Buffer( istreambuf_iterator<char>(t_Input), (istreambuf_iterator<char>()) );

		cout << t_FileName << " Loaded. Size = " << t_Buffer.size() << " Byte" << endl;

		// Sanitize Filename if in other directory
		if (t_FileName.find('/') != string::npos || t_FileName.find('\\') != string::npos)
			if (t_FileName.find('/') != string::npos)
				t_FileName = t_FileName.substr(t_FileName.find_last_of('/') + 1);
			else
				t_FileName = t_FileName.substr(t_FileName.find_last_of('\\') + 1);

		auto t_Type = "RGBA";

		vector<unsigned char> t_Pixels;
		if (argc >= 3 && !strcmp(args[2], "RGB"))
		{
			// To "RGB"
				for (int i = 0; i < t_Buffer.size(); i++)
				{
					t_Pixels.push_back(t_Buffer[i]);

					if ((i + 1) % 3 == 0 && i != 0)
						t_Pixels.push_back(255);
				}
			// Set Type for Filename
				t_Type = "RGB";
		}
		else // Keep current RGBA format
			t_Pixels = t_Buffer;

		// Calculate Square Image properties from data
			auto t_Width =	int( sqrt( (t_Pixels.size() / 4) ) );
			auto t_Height = int( sqrt( (t_Pixels.size() / 4) ) );

		// Final Output Filename
			t_FileName = t_FileName + "_" + t_Type + "_Visualized.png";

		cout << "Creating Image: " << t_FileName << " (" << t_Width << "x" << t_Height << ")" << endl;

		// Create/Encode Image
			auto t_Error = lodepng::encode(t_FileName, t_Pixels, t_Width, t_Height);

		// Check for Errors
			if (t_Error)
				cout << "Encode Error(" << t_Error << "): " << lodepng_error_text(t_Error) << endl;
			else
				cout << "Successfully generated image: " << t_FileName << endl;

	}
	else
		cout << "Could not open file: " << args[0] << endl;
	
	return 0;
}