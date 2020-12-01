#include"Foothold.h"

void Foothold::Init() {
	Move = glm::mat4(1.0f);
	Scale = glm::mat4(1.0f);
	Rotate = glm::mat4(1.0f);

	Move = glm::translate(Move, glm::vec3(mx, my, mz));
	Scale = glm::scale(Scale, glm::vec3(cx, cy, cz));
	Rotate = glm::rotate(Rotate, glm::radians(theta), glm::vec3(rx, ry, rz));
}

void Foothold::Pos_Drawing() {	
	Drawing = Move * Scale; //애니메이션/카메라 추가 필요
}

void Foothold::Draw_Start() {
	Init();
	Pos_Drawing();
}

void  Foothold::Delete() {
	switch (ani) {
	case 0:
		cx -= 0.1;
		cz -= 0.1;
		if (cx <= 0 && cz <= 0)
			Del = TRUE;
		break;
	case 1:
		r += 0.05;
		g += 0.05;
		b += 0.05;
		if (r >= 1 && g >= 1 && b >= 1)
			Del = TRUE;
	}
}

void MakeFoothold(vector<Foothold> &Bottom)
{
	srand(time(0));
	float r, g, b;

	for (int k = 0; k < N; ++k) {
		r = rand()/MAX;
		g = rand()/MAX;
		b = rand()/MAX;
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				Bottom.push_back(Foothold(-2.0f + (foothold_sizex + 0.1f) * j, 2.0f - 5.0f * k, -2.0f + (foothold_sizez + 0.1f) * i
					, r, g, b));
			}	// -화면크기 + (발판사이즈 + 간격) 
		}
	}
}
