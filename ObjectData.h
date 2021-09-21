#ifndef OBJECT_DATA_H
#define OBJECT_DATA_H

//============================================================================================
//↓GLFWを使うためにライブラリファイルを指定(プロパティからパスを指定してもよい)==============
#pragma comment(lib, "C:\\glfw-3.3.2.bin.WIN64\\lib-vc2017\\glfw3.lib")
#pragma comment(lib, "C:\\glfw-3.3.2.bin.WIN64\\lib-vc2017\\glfw3dll.lib")
#pragma comment(lib,"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\OpenGL32.lib")
#pragma comment(lib,"C:\\Program Files (x86)\\Windows Kits\\10\\Lib\\10.0.17763.0\\um\\x64\\GlU32.Lib")
//============================================================================================

#define NUM_OF_SURFACE	(100000)
#define	DIM_3			(3)
#define	DIM_6			(6)
#define PAT_ROWS		(7)
#define	PAT_COLS		(10)
#define RB_LINK_NUM		(7)
#define BASE_OFFSET_X	(float)(0.0)
#define BASE_OFFSET_Y	(float)(0.0)
#define BASE_OFFSET_Z	(float)(97.40)

namespace CG {

	typedef struct {
		float	x;
		float	y;
		float	z;
	} VECTOR;

	typedef struct {
		VECTOR NormalVector;
		VECTOR TrianglePoint[DIM_3];
	} SurfaceData;

	typedef struct {
		unsigned int N;
		SurfaceData Surface[NUM_OF_SURFACE];
	} STL_ObjectData;

	//CADモデルのカラー
	typedef struct {
		double red;
		double green;
		double blue;
	} CADColor;

	typedef struct {
		CADColor		LinkColor[RB_LINK_NUM];
		STL_ObjectData	LinkData[RB_LINK_NUM];
	} RBModel;

	class CGROBOT {
	public:
		//コンストラクタ
		CGROBOT() {};

		CGROBOT(float x, float y) {
			FieldX = x;
			FieldY = y;
		}

		//デストラクタ
		virtual ~CGROBOT() {};

		//公開関数
		void SetLighting(void);
		void Read_STData(const char *FileName, STL_ObjectData *Object);
		void DrawSTL(STL_ObjectData *Object, CADColor *color);
		void DrawField(void);
		void DrawCoodinate(float AxisSize);
		void DrawChessBoard(float PatternSize);
		void DrawScaraRB(RBModel *Model, float *JointAngle, VECTOR *HandPos);

	private:
		//プライベート関数
		
		//プライベート変数
		float FieldX = 1000;//[mm]
		float FieldY = 1000;//[mm]
		float light0_position[4] = { 0.0, 0.0, 1.0, 0.0 };
		float lightAmbient[3] = { 0.25, 0.25, 0.25 };
	};

}
#endif