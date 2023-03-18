#include"Precompiled.h"
#include"MeshBuilder.h"
#include<math.h>
using namespace sqEngine;
using namespace sqEngine::Graphics;
using namespace sqEngine::Math;

MeshPC MeshBuilder::CreateCube()
{
	MeshPC mesh;

	mesh.vertices.push_back({ { -0.5f,  0.5f, 0.0f }, Colors::AliceBlue });//0
	mesh.vertices.push_back({ {  0.5f,  0.5f, 0.0f }, Colors::MintCream });//1
	mesh.vertices.push_back({ { -0.5f, -0.5f, 0.0f }, Colors::SandyBrown });//2
	mesh.vertices.push_back({ { 0.5f, -0.5f, 0.0f }, Colors::RosyBrown });//3
	mesh.vertices.push_back({ { -0.5f,  0.5f, 1.0f }, Colors::AliceBlue });//4
	mesh.vertices.push_back({ {  0.5f,  0.5f, 1.0f }, Colors::MintCream });//5
	mesh.vertices.push_back({ { -0.5f, -0.5f, 1.0f }, Colors::AliceBlue });//6
	mesh.vertices.push_back({ { 0.5f, -0.5f, 1.0f }, Colors::RosyBrown });//7

	
	mesh.indices = { 0,1,2,2,1,3,4,5,6,6,5,7,4,5,0,5,1,0,6,2,3,7,6,3,4,0,2,4,2,6,5,3,1,5,7,3 };
	return mesh;
}

MeshPC MeshBuilder::CreatePlanePc(int row, int column,float spacing)
{
	MeshPC mesh;
	int n = 0;

	for (int j = 0; j < column; ++j)
	{
		for (int i = 0; i < row; ++i)
		{
			mesh.vertices.push_back({ { i*spacing, 0.0f, (j+1)*spacing }, Colors::AliceBlue });//0
			mesh.vertices.push_back({ { i * spacing, 0.0f, j * spacing }, Colors::RosyBrown });
			mesh.vertices.push_back({ { (i+1) * spacing, 0.0f, j * spacing }, Colors::AliceBlue });
			mesh.vertices.push_back({ { (i + 1)* spacing, 0.0f, (j + 1) * spacing }, Colors::RosyBrown});
		}
	}


	for (int j = 0; j < column; ++j)
	{
		for (int i = 0; i < row; ++i)
		{
			n = 4 * i + 4 * j * (row);
			//mesh.indices.push_back(uint32_t(i+j), uint32_t(i+j+1), uint32_t(i + j + 2), uint32_t(i + j + 3));
			mesh.indices.push_back(uint32_t(n));
			mesh.indices.push_back(uint32_t(2 +n));
			mesh.indices.push_back(uint32_t(1 +n));

			mesh.indices.push_back(uint32_t(n));
			mesh.indices.push_back(uint32_t(3+n));
			mesh.indices.push_back(uint32_t(2+n));		
		}
	}
	return mesh;
}



MeshPC MeshBuilder::Createcylinder(int segment, int sides, float radius)
{
	MeshPC mesh;
	float angle = 2 * (3.1415926f)/ segment;
	float thickness = 0.01f;
	
	for (int j = 0; j < sides; ++j)
	{
		for (int i = 0; i < segment; ++i)
		{
			if (i == segment -1)
			{
				mesh.vertices.push_back({ { sin(-angle) * radius,0 + j * thickness, cos(- angle) * radius + 0.5f }, Colors::RosyBrown });
				mesh.vertices.push_back({ { (float)sin(0) * radius,0 + j * thickness, (float)cos(0) * radius + 0.5f }, Colors::AliceBlue });
				mesh.vertices.push_back({ { 0,0 + j * thickness,0.5 }, Colors::AliceBlue });//原点
			}
			else
			{
				mesh.vertices.push_back({ { sin(i * angle) * radius,0 + j * thickness, cos(i * angle) * radius + 0.5f }, Colors::AliceBlue });
				mesh.vertices.push_back({ { sin((i + 1) * angle) * radius,0 + j * thickness, cos((i + 1) * angle) * radius + 0.5f }, Colors::RosyBrown });
				mesh.vertices.push_back({ { 0,0 + j * thickness,0.5 }, Colors::AliceBlue });//原点
			}
		}
	}

	for (int j = 0; j < sides; ++j)
	{
		for (int i = 0; i < segment; ++i)
		{
			float n = 3 * i + 3*j*segment;
			mesh.indices.push_back(uint32_t(n));
			mesh.indices.push_back(uint32_t(1 + n ));
			mesh.indices.push_back(uint32_t(2 + n ));
		}
	}
	return mesh;
}


MeshPC MeshBuilder::CreateSpherePC(float radius, int rings, int slices)
{
	MeshPC mesh;
	Color color[] = { Colors::Red,Colors::Azure,Colors::Green,Colors::Yellow };

	const float ringAngle = Math::Constants::Pi / (rings - 1);
	const float sliceAngle = Math::Constants::TwoPi / slices;

	//从低点向上画到顶点 通过ringAngle判断 顶底点各画slices次
	for (uint32_t r = 0; r < rings; ++r)
	{
		const float phi = r * ringAngle;
		const float y = -cos(phi) * radius;
		const float newRadius = sin(phi) * radius;

		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float x = -sin(s * sliceAngle) * newRadius;
			const float z = cos(s * sliceAngle) * newRadius;

			mesh.vertices.push_back({ {x,y,z},color[s % 4] });
		}
	}

	for (uint32_t r = 0; r < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			//先通过单个低点  连接上环两点 r判断环层 
			mesh.indices.push_back(s + r * (slices + 1));//底1
			mesh.indices.push_back(s + (r + 1) * (slices + 1));//上方1
			mesh.indices.push_back(s + 1 + (r + 1) * (slices + 1));//上方2
		}
	}

	for (uint32_t r = 1; r < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			//在通过r+1层两点  连接上方一点 
			mesh.indices.push_back(s + r * (slices + 1));//底+1层点1 与底1同位置
			mesh.indices.push_back(s + 1 + (r + 1) * (slices + 1));//底+2层(上方) 与 上方2 底+1层点2 同位置
			mesh.indices.push_back(s + 1 + r * (slices + 1));//底+1层点2 
		}
	}
	return mesh;
}

MeshPX MeshBuilder::CreateSkyBoxPX(float size)
{
	MeshPX mesh;
	//背
	mesh.vertices.push_back({ {-size,-size,size}, 0, 1 });
	mesh.vertices.push_back({ {-size,size,size}, 0, 0 });
	mesh.vertices.push_back({ {size,size,size},1, 0 });

	mesh.vertices.push_back({ {-size,-size,size}, 0, 1 });
	mesh.vertices.push_back({ {size,size,size}, 1, 0 });
	mesh.vertices.push_back({ {size,-size,size}, 1, 1 });

	mesh.indices.push_back(0);
	mesh.indices.push_back(1);
	mesh.indices.push_back(2);
	mesh.indices.push_back(3);
	mesh.indices.push_back(4);
	mesh.indices.push_back(5);

	//右
	mesh.vertices.push_back({ {size,-size,size}, 0, 1 });
	mesh.vertices.push_back({ {size,size,size}, 0, 0 });
	mesh.vertices.push_back({ {size,size,-size}, 1, 0 });
	mesh.indices.push_back(6);
	mesh.indices.push_back(7);
	mesh.indices.push_back(8);
	mesh.vertices.push_back({ {size,-size,size}, 0, 1 });
	mesh.vertices.push_back({ {size,size,-size}, 1, 0 });
	mesh.vertices.push_back({ {size,-size,-size}, 1, 1 });
	mesh.indices.push_back(9);
	mesh.indices.push_back(10);
	mesh.indices.push_back(11);

	//左
	mesh.vertices.push_back({ {-size,-size,-size}, 0, 1 });
	mesh.vertices.push_back({ {-size,size,-size}, 0, 0 });
	mesh.vertices.push_back({ {-size,size,size}, 1, 0 });
	mesh.indices.push_back(12);
	mesh.indices.push_back(13);
	mesh.indices.push_back(14);
	mesh.vertices.push_back({ {-size,-size,-size}, 0, 1 });
	mesh.vertices.push_back({ {-size,size,size}, 1, 0 });
	mesh.vertices.push_back({ {-size,-size,size}, 1, 1 });
	mesh.indices.push_back(15);
	mesh.indices.push_back(16);
	mesh.indices.push_back(17);
	//上
	mesh.vertices.push_back({ {-size,size,size}, 0, 1 });
	mesh.vertices.push_back({ {-size,size,-size}, 0, 0 });
	mesh.vertices.push_back({ {size,size,-size}, 1, 0 });
	mesh.vertices.push_back({ {-size,size,size}, 0, 1 });
	mesh.vertices.push_back({ {size,size,-size}, 1, 0 });
	mesh.vertices.push_back({ {size,size,size}, 1, 1 });
	mesh.indices.push_back(18);
	mesh.indices.push_back(19);
	mesh.indices.push_back(20);
	mesh.indices.push_back(21);
	mesh.indices.push_back(22);
	mesh.indices.push_back(23);
	//下
	mesh.vertices.push_back({ {-size, -size,size}, 0, 1 });//1
	mesh.vertices.push_back({ { size, -size,-size}, 1, 0 });//3
	mesh.vertices.push_back({ {-size, -size,-size}, 0, 0 });//2
	mesh.vertices.push_back({ {-size, -size,size}, 0, 1 });//
	mesh.vertices.push_back({ { size, -size,size}, 1, 1 });
	mesh.vertices.push_back({ { size, -size,0 - size}, 1, 0 });
	mesh.indices.push_back(24);
	mesh.indices.push_back(25);
	mesh.indices.push_back(26);
	mesh.indices.push_back(27);
	mesh.indices.push_back(28);
	mesh.indices.push_back(29);

	//正
	mesh.vertices.push_back({ {-size,-size,-size}, 0, 1 });
	mesh.vertices.push_back({ {size,size,-size},1, 0 });
	mesh.vertices.push_back({ {-size,size,-size}, 0, 0 });

	mesh.vertices.push_back({ {-size,-size,-size}, 0, 1 });
	mesh.vertices.push_back({ {size,-size,-size}, 1, 1 });
	mesh.vertices.push_back({ {size,size,-size}, 1, 0 });
	mesh.indices.push_back(30);
	mesh.indices.push_back(31);
	mesh.indices.push_back(32);
	mesh.indices.push_back(33);
	mesh.indices.push_back(34);
	mesh.indices.push_back(35);
	return mesh;
}

MeshPX MeshBuilder::CreateSkyBox()
{
	MeshPX mesh;
	return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(float radius, int rings, int slices)
{
	MeshPX mesh;

	const float ringStep = Math::Constants::Pi / (rings - 1);
	const float sliceStep = Math::Constants::TwoPi / slices;

	const float uPercentage = 1.0f / slices;
	const float vPercentage = 1.0f / (rings - 1);

	for (uint32_t r = 0; r < rings; ++r)
	{
		const float phi = r * ringStep;
		const float y = -cos(phi) * radius;
		const float newRadius = sin(phi) * radius;
		const float v = 1.0f - r * vPercentage;

		for (uint32_t s = 0; s <= slices; ++s)
		{
			const float x = -sin(s * sliceStep) * newRadius;
			const float z = cos(s * sliceStep) * newRadius;
			const float u = s * uPercentage;

			mesh.vertices.push_back({ {x,y,z}, u, v });
		}
	}

	for (uint32_t r = 0; r + 1 < rings; ++r)
	{
		for (uint32_t s = 0; s < slices; ++s)
		{
			mesh.indices.push_back((s + 0) + (r + 0) * (slices + 1));
			mesh.indices.push_back((s + 0) + (r + 1) * (slices + 1));
			mesh.indices.push_back((s + 1) + (r + 1) * (slices + 1));

			mesh.indices.push_back((s + 0) + (r + 0) * (slices + 1));
			mesh.indices.push_back((s + 1) + (r + 1) * (slices + 1));
			mesh.indices.push_back((s + 1) + (r + 0) * (slices + 1));
		}
	}
	return mesh;
}

MeshPX MeshBuilder::CreateSkyDome(float radius, int rings, int slices)
{
	MeshPX mesh;
	return mesh;
}

MeshPX MeshBuilder::CreateScreenQuad()
{
	// (0, 0)          (1, 0)    
	//  1---------------2
	//  +          .    +
	//  +		.		+
	//  +	.			+
	//  0---------------3
	// (0, 1)          (1, 1)
	MeshPX mesh;
	mesh.vertices.push_back({ {-1.0f, -1.0f, 0.0f}, 0.0f, 1.0f });
	mesh.vertices.push_back({ {-1.0f, +1.0f, 0.0f}, 0.0f, 0.0f });
	mesh.vertices.push_back({ {+1.0f, +1.0f, 0.0f}, 1.0f, 0.0f });
	mesh.vertices.push_back({ {+1.0f, -1.0f, 0.0f}, 1.0f, 1.0f });
	mesh.indices = { 0, 1, 2, 0, 2, 3 };
	return mesh;
}

Mesh MeshBuilder::CreateSphere(int slices, int rings, float radius)
{
	ASSERT(slices >= 3 && rings >= 2, "Invalid arguments for sphere.");
	ASSERT(radius > 0.0f, "Invalid arguments for sphere.");

	Mesh mesh;

	const uint32_t columns = slices + 1;
	const uint32_t rows = rings;
	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6;
	const float width = 0.0f;
	const float length = 0.0f;
	const float thetaStep = Math::Constants::TwoPi / slices;
	const float phiStep = Math::Constants::Pi / (rings - 1);
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);

	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float theta = c * thetaStep;
			const float phi = r * phiStep;
			const float discRadius = radius * sin(phi);
			const float x = -sin(theta) * discRadius;
			const float y = -cos(phi) * radius;
			const float z = cos(theta) * discRadius;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);
			const Vector3 p = { x,y,z };
			const Vector3 n = Normalize(p);
			const Vector3 t = Normalize(Math::Vector3{-z,0.0f,x});
			mesh.vertices.push_back({p,n,t,u,v});
		}
	}

	for (uint32_t r = 0; r < rows -1; ++r)
	{
		for (uint32_t s = 0; s < columns -1; ++s)
		{
			//先通过单个低点  连接上环两点 r判断环层 
			mesh.indices.push_back(s + r * (slices + 1));//底1
			mesh.indices.push_back(s + (r + 1) * (slices + 1));//上方1
			mesh.indices.push_back(s + 1 + (r + 1) * (slices + 1));//上方2
		}
	}

	for (uint32_t r = 1; r < rows - 1; ++r)
	{
		for (uint32_t s = 0; s < columns - 1; ++s)
		{
			//在通过r+1层两点  连接上方一点 
			mesh.indices.push_back(s + r * (slices + 1));//底+1层点1 与底1同位置
			mesh.indices.push_back(s + 1 + (r + 1) * (slices + 1));//底+2层(上方) 与 上方2 底+1层点2 同位置
			mesh.indices.push_back(s + 1 + r * (slices + 1));//底+1层点2 
		}
	}
	return mesh;
}

Mesh MeshBuilder::CreatePlane(int rows, int columns, float spacing)
{
	Mesh mesh;
	const uint32_t vertexCount = columns * rows;
	const uint32_t indexCount = (columns - 1) * (rows - 1) * 6;
	const float offsetX = (columns - 1)*spacing* -0.5f;
	const float offsetZ = (rows - 1) * spacing * -0.5f;
	const float uStep = 1.0f / (columns - 1);
	const float vStep = 1.0f / (rows - 1);

	mesh.vertices.reserve(vertexCount);
	mesh.indices.reserve(indexCount);

	//add vertices
	for (uint32_t r = 0; r < rows; ++r)
	{
		for (uint32_t c = 0; c < columns; ++c)
		{
			const float x = (c * spacing) + offsetX;
			const float y = 0;
			const float z = (r * spacing) + offsetZ;
			const float u = c * uStep;
			const float v = 1.0f - (r * vStep);
			const Vector3 p = { x,0,z };
			const Vector3 n = Math::Vector3::YAxis;
			const Vector3 t = Math::Vector3::XAxis;
			mesh.vertices.push_back({ p,n,t,u,v });
		}
	}

	//add indices
	for (uint32_t r = 0; r < rows - 1; ++r)
	{
		for (uint32_t c = 0; c < columns - 1; ++c)
		{
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 0) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
											 
			mesh.indices.push_back((c + 0) + ((r + 0) * columns));
			mesh.indices.push_back((c + 1) + ((r + 1) * columns));
			mesh.indices.push_back((c + 1) + ((r + 0) * columns));
		}
	}

	return mesh;
}
