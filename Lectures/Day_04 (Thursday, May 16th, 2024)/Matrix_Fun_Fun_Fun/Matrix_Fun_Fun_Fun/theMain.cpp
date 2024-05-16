#include <iostream>

struct sVector3f
{
	float x, y, z;
	sVector3f() {
		this->x = this->y = this->z = 0.0f;
	}
};

struct sVector4f
{
	float x, y, z, w;
	sVector4f() {
		this->x = this->y = this->z = 0.0f;
		this->w = 1.0f;	// If unsure, make 4th value 1.0
	}
};

struct sMatrix4x4
{
	//  x[0]   x[1]   x[2]   x[3]
	//  y[0]   y[1]   y[2]   y[3]
	//  z[0]   z[1]   z[2]   z[3]
	//  w[0]   w[1]   w[2]   w[3]
	float x[4];
	float y[4];
	float z[4];
	float w[4];
	sMatrix4x4()
	{
		this->x[0] = this->x[1] = this->x[2] = this->x[3] = 0.0f;
		this->y[0] = this->y[1] = this->y[2] = this->y[3] = 0.0f;
		this->z[0] = this->z[1] = this->z[2] = this->z[3] = 0.0f;
		this->w[0] = this->w[1] = this->w[2] = this->w[3] = 0.0f;
	}
};

void mat4x4_multiply( sMatrix4x4 &a, sMatrix4x4 &b, sMatrix4x4 &result )
{
	//  x[0]   x[1]   x[2]   x[3]
	//  y[0]   y[1]   y[2]   y[3]
	//  z[0]   z[1]   z[2]   z[3]
	//  w[0]   w[1]   w[2]   w[3]


//    ___       _   _     _                                    _  ___ 
//   |_ _|___  | |_| |__ (_)___    ___ ___  _ __ _ __ ___  ___| ||__ \
//    | |/ __| | __| '_ \| / __|  / __/ _ \| '__| '__/ _ \/ __| __|/ /
//    | |\__ \ | |_| | | | \__ \ | (_| (_) | |  | | |  __/ (__| |_|_| 
//   |___|___/  \__|_| |_|_|___/  \___\___/|_|  |_|  \___|\___|\__(_) 
//                                                                    


	result.x[0] = (a.x[0] * b.x[0]) + (a.x[1] * b.y[0]) + (a.x[2] * b.z[0]) + (a.x[3] * b.w[0]);
	result.x[1] = (a.x[0] * b.x[1]) + (a.x[1] * b.y[1]) + (a.x[2] * b.z[1]) + (a.x[3] * b.w[1]);
	result.x[2] = (a.x[0] * b.x[2]) + (a.x[1] * b.y[2]) + (a.x[2] * b.z[2]) + (a.x[3] * b.w[2]);
	result.x[3] = (a.x[0] * b.x[3]) + (a.x[1] * b.y[3]) + (a.x[2] * b.z[3]) + (a.x[3] * b.w[3]);

	result.y[0] = (a.y[0] * b.x[0]) + (a.y[1] * b.y[0]) + (a.y[2] * b.z[0]) + (a.y[3] * b.w[0]);
	result.y[1] = (a.y[0] * b.x[1]) + (a.y[1] * b.y[1]) + (a.y[2] * b.z[1]) + (a.y[3] * b.w[1]);
	result.y[2] = (a.y[0] * b.x[2]) + (a.y[1] * b.y[2]) + (a.y[2] * b.z[2]) + (a.y[3] * b.w[2]);
	result.y[3] = (a.y[0] * b.x[3]) + (a.y[1] * b.y[3]) + (a.y[2] * b.z[3]) + (a.y[3] * b.w[3]);

	result.z[0] = (a.z[0] * b.x[0]) + (a.z[1] * b.y[0]) + (a.z[2] * b.z[0]) + (a.z[3] * b.w[0]);
	result.z[1] = (a.z[0] * b.x[1]) + (a.z[1] * b.y[1]) + (a.z[2] * b.z[1]) + (a.z[3] * b.w[1]);
	result.z[2] = (a.z[0] * b.x[2]) + (a.z[1] * b.y[2]) + (a.z[2] * b.z[2]) + (a.z[3] * b.w[2]);
	result.z[3] = (a.z[0] * b.x[3]) + (a.z[1] * b.y[3]) + (a.z[2] * b.z[3]) + (a.z[3] * b.w[3]);

	result.w[0] = (a.w[0] * b.x[0]) + (a.w[1] * b.y[0]) + (a.w[2] * b.z[0]) + (a.w[3] * b.w[0]);
	result.w[1] = (a.w[0] * b.x[1]) + (a.w[1] * b.y[1]) + (a.w[2] * b.z[1]) + (a.w[3] * b.w[1]);
	result.w[2] = (a.w[0] * b.x[2]) + (a.w[1] * b.y[2]) + (a.w[2] * b.z[2]) + (a.w[3] * b.w[2]);
	result.w[3] = (a.w[0] * b.x[3]) + (a.w[1] * b.y[3]) + (a.w[2] * b.z[3]) + (a.w[3] * b.w[3]);


	return;
}

void vec4_x_mat4x4( sVector4f &v, sMatrix4x4 &m, sVector4f &result )
{
	result.x = (m.x[0] * v.x) + (m.x[1] * v.y) + (m.x[2] * v.z) + (m.x[3] * v.w);
	result.x = (m.y[0] * v.x) + (m.y[1] * v.y) + (m.y[2] * v.z) + (m.y[3] * v.w);
	result.z = (m.z[0] * v.x) + (m.z[1] * v.y) + (m.z[2] * v.z) + (m.z[3] * v.w);
	result.w = (m.w[0] * v.x) + (m.w[1] * v.y) + (m.w[2] * v.z) + (m.w[3] * v.w);

	return;
}


int main()
{





	return 0;
}
