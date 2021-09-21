#include <stdio.h>
#include "ObjectData.h"
#include "C:\glfw-3.3.2.bin.WIN64\include\GLFW\glfw3.h"

namespace CG {

	void CGROBOT::SetLighting(void) {
		
		glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightAmbient);
		glEnable(GL_LIGHTING);	//照明モデルを有効にする
		glEnable(GL_LIGHT0);	//光源の0番目を有効にする(0番目～7番目まで使用可能．同時に複数の光源を設定できるらしい)
		glEnable(GL_COLOR_MATERIAL);	//物体の質感を有効にする
	}

	//STLデータの読み込み
	void CGROBOT::Read_STData(const char *FileName, STL_ObjectData *Object) {
		FILE *fp;
		char byte80[80];	//STLファイルの最初のコードを格納する
		unsigned short byte2;	//
		unsigned int n = 0;	//
		float x = 0;
		int i = 0, j = 0;

		//ファイルを開く
		fp = fopen(FileName, "rb");

		//最初の余分な情報だけ読み込みを進める
		fread(byte80, sizeof(char), 80, fp);

		//面の総数を読み込む
		fread(&Object->N, sizeof(unsigned int), 1, fp);
		printf("Object->N:%d\n", Object->N);

		//オブジェクトの情報を読み込む
		for (i = 0; i < Object->N; i++) {
			//法線ベクトルの読み込み
			fread(&Object->Surface[i].NormalVector.x, sizeof(float), 1, fp);
			fread(&Object->Surface[i].NormalVector.y, sizeof(float), 1, fp);
			fread(&Object->Surface[i].NormalVector.z, sizeof(float), 1, fp);

			//三角面を成す3頂点を読み込む
			for (j = 0; j < 3; j++) {
				fread(&Object->Surface[i].TrianglePoint[j].x, sizeof(float), 1, fp);
				fread(&Object->Surface[i].TrianglePoint[j].y, sizeof(float), 1, fp);
				fread(&Object->Surface[i].TrianglePoint[j].z, sizeof(float), 1, fp);
			}
			//余分な情報の読み込みを進める
			fread(&byte2, sizeof(unsigned short), 1, fp);
		}
		fclose(fp);
	}

	//STLデータの表示
	void CGROBOT::DrawSTL(STL_ObjectData *Object, CADColor *color) {
		int i = 0;
		int j = 0;

		glColor3f(color->red, color->green, color->blue);
		glLineWidth(2.0);
		for (i = 0; i < Object->N; i++) {
			glBegin(GL_POLYGON);
			glNormal3f(Object->Surface[i].NormalVector.x, Object->Surface[i].NormalVector.y, Object->Surface[i].NormalVector.z);//面の法線を指定
			for (j = 0; j < 3; j++) {
				glVertex3f(Object->Surface[i].TrianglePoint[j].x, Object->Surface[i].TrianglePoint[j].y, Object->Surface[i].TrianglePoint[j].z);//面のを描画
			}
			glEnd();
		}

	}

	//平面の描画
	void CGROBOT::DrawField(void) {
		glLineWidth(2.0);
		glBegin(GL_POLYGON);
		glColor3d(0., 0.5, 0.);
		glVertex3d(-FieldX, -FieldY, 0);
		glVertex3d(FieldX, -FieldY, 0);
		glVertex3d(FieldX, FieldY, 0);
		glVertex3d(-FieldX, FieldY, 0);
		glEnd();
	}

	//座標系の表示
	void CGROBOT::DrawCoodinate(float AxisSize) {
		glLineWidth(3.0);//線の太さを指定
		//線の描画開始
		//線をたくさん書くときは，GL_LINESで
		glBegin(GL_LINES);
		//x軸(赤)
		glColor3d(1., 0., 0.);
		glVertex3d(0., 0., 0.);//線の始点(x,y,z)
		glVertex3d(AxisSize, 0., 0.);
		//y軸(緑)
		glColor3d(0., 1., 0.);
		glVertex3d(0., 0., 0.);//線の始点(x,y,z)
		glVertex3d(0., AxisSize, 0.);
		//z軸(青)
		glColor3d(0., 0., 1.);
		glVertex3d(0., 0., 0.);//線の始点(x,y,z)
		glVertex3d(0., 0., AxisSize);
		glEnd();//線の描画終了
	}

	//チェスボードの描画
	void CGROBOT::DrawChessBoard(float PatternSize) {
		int num = 0;//座標の番号

		//描画
		num = 0;
		for (int i = 0; i < PAT_ROWS - 1; i++)
		{
			for (int j = 0; j < PAT_COLS - 1; j++)
			{
				if (num % 2 == 0) {//格子の左上が偶数
					glColor3d(0., 0., 0.);
					glBegin(GL_POLYGON);
					glVertex3d(i * PatternSize, j * PatternSize, 1);
					glVertex3d(i * PatternSize, (j + 1) * PatternSize, 1);
					glVertex3d((i + 1) * PatternSize, (j + 1) * PatternSize, 1);
					glVertex3d((i + 1) * PatternSize, j * PatternSize, 1);
					glEnd();
				}
				else {
					glColor3d(1., 1., 1.);
					glBegin(GL_POLYGON);
					glVertex3d(i * PatternSize, j * PatternSize, 1);
					glVertex3d(i * PatternSize, (j + 1) * PatternSize, 1);
					glVertex3d((i + 1) * PatternSize, (j + 1) * PatternSize, 1);
					glVertex3d((i + 1) * PatternSize, j * PatternSize, 1);
					glEnd();
				}
				num++;
			}
		}

	}

	//SCARAロボットの描画
	void CGROBOT::DrawScaraRB(RBModel *Model, float *JointAngle, VECTOR *HandPos) {
		//ロボットベースの描画
		glPushMatrix();
		glTranslated(BASE_OFFSET_X, BASE_OFFSET_Y, BASE_OFFSET_Z);
		DrawSTL(&Model->LinkData[0], &Model->LinkColor[0]);

		//アーム1の描画
		glTranslated(0, 0, 0);
		glRotated(JointAngle[0], 0, 0, 1);
		DrawCoodinate(20);
		DrawSTL(&Model->LinkData[1], &Model->LinkColor[1]);

		//アーム2の描画
		glTranslated(75.0, 0, 0);
		glRotated(JointAngle[1], 0, 0, 1);
		DrawCoodinate(20);
		DrawSTL(&Model->LinkData[2], &Model->LinkColor[2]);
		glPopMatrix();

		//エンドエフェクタの描画
		glPushMatrix();
		glTranslated(HandPos->x, HandPos->y, HandPos->z);
		DrawCoodinate(50);
		glPopMatrix();
	}
}