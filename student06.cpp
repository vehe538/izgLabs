// @author ~ Marek Čupr (xcuprm01)
// @body ~ 3/3

/******************************************************************************
 * Laborator 04 Krivky - Zaklady pocitacove grafiky - IZG
 * ihulik@fit.vutbr.cz
 *
 *
 * Popis: Hlavicky funkci pro funkce studentu
 *
 * Opravy a modifikace:
 * - isvoboda@fit.vutbr.cz
 * - ireich@fit.vutbr.cz
 */

#include "student.h"
#include "globals.h"
#include "main.h"

#include <time.h>
#include <math.h>

 //Viz hlavicka vector.h
USE_VECTOR_OF(Point2d, point2d_vec)
#define point2d_vecGet(pVec, i) (*point2d_vecGetPtr((pVec), (i)))

/* Secte dva body Point2d a vysledek vrati v result (musi byt inicializovan a alokovan)*/

// PRE-DEFINED FUNKCE (nic nemenime)
void	addPoint2d(const Point2d* a, const Point2d* b, Point2d* result)
{
	IZG_ASSERT(result);
	result->x = a->x + b->x;
	result->y = a->y + b->y;
}

/* Vynasobi bod Point2d skalarni hodnotou typu double a vysledek vrati v result (musi byt inicializovan a alokovan)*/

// PRE-DEFINED FUNKCE (nic nemenime)
void	mullPoint2d(double val, const Point2d* p, Point2d* result)
{
	IZG_ASSERT(result);
	result->x = p->x * val;
	result->y = p->y * val;
}

/**
 * Inicializace ridicich bodu horni trajektorie ve vykreslovacim okne.
 * Pocatek souradnicove soustavy je v levem hornim rohu okna. Xova souradnice
 * roste smerem doprava, Y smerem dolu.
 * @param points vystup, kam se pridavaji ridici body
 * @param offset_x posun vsech ridicich bodu v horizontalnim smeru (aby se mohli souradnice zadavat s pocatkem [0,0])
 * @param offset_y posun vsech ridicich bodu ve vertikalnim smeru
 */

// PRE-DEFINED FUNKCE (nic nemenime)
void initControlPointsUp(S_Vector** points, int offset_x, int offset_y) {
	*points = vecCreateEmpty(sizeof(Point2d));
	Point2d p;
	p.x = 0;   p.y = 0;    point2d_vecPushBack(*points, p);

	p.x = 40;  p.y = -250; point2d_vecPushBack(*points, p);
	p.x = 160; p.y = -250; point2d_vecPushBack(*points, p);

	p.x = 200; p.y = 0;    point2d_vecPushBack(*points, p);

	p.x = 210; p.y = -180; point2d_vecPushBack(*points, p);
	p.x = 350; p.y = -180; point2d_vecPushBack(*points, p);

	p.x = 360; p.y = 0;    point2d_vecPushBack(*points, p);

	p.x = 390; p.y = -120;  point2d_vecPushBack(*points, p);
	p.x = 430; p.y = -120;  point2d_vecPushBack(*points, p);

	p.x = 460; p.y = 0;    point2d_vecPushBack(*points, p);

	p.x = 470; p.y = -70;  point2d_vecPushBack(*points, p);
	p.x = 525; p.y = -70;  point2d_vecPushBack(*points, p);

	p.x = 535; p.y = 0;    point2d_vecPushBack(*points, p);

	p.x = 545; p.y = -40;  point2d_vecPushBack(*points, p);
	p.x = 575; p.y = -40;  point2d_vecPushBack(*points, p);

	p.x = 585; p.y = 0;    point2d_vecPushBack(*points, p);

	Point2d offset = { static_cast<double>(offset_x), static_cast<double>(offset_y), 1.0 };
	for (int i = 0; i < (*points)->size; i++) {
		addPoint2d(point2d_vecGetPtr(*points, i), &offset, point2d_vecGetPtr(*points, i));
	}
}


void initControlPointsDown(S_Vector** points, int offset_x, int offset_y) {
	/* == TODO ==
	 * Uloha c.2
	 * Nasledujuci volanni funkce initControlPointsUp(.) zmazte a nahradte vlastnim kodem,
	 * ktery inicializuje ridici body tak, aby byla trajektorie spojita (C1). Muzete zkopirovat
	 * kod funkce initControlPointsUp(.) a upravit primo souradnice bodu v kodu. 
         * POCITEJTE S -Y SOURADNICEMI RIDICICH BODU A MENTE JEJICH X SLOŽKU!
	 */

	// KOD NIZE JE CELY ZKOPIROVANY Z FUNKCE initControlPointsUp, menime zde pouze 4 radky
	// POINTA TEHLE FUNKCE JE TO, ZE KAZDY DRUHY OBLOUK PREKLOPIME PODLE OSY Y
	*points = vecCreateEmpty(sizeof(Point2d));
	Point2d p;
	p.x = 0;   p.y = 0;    point2d_vecPushBack(*points, p); // zacatek krivky

	p.x = 40;  p.y = -250; point2d_vecPushBack(*points, p); // prvni oblouk nechavame (nemeni nic)
	p.x = 160; p.y = -250; point2d_vecPushBack(*points, p); // prvni oblouk nechavame (nemeni nic)

	p.x = 200; p.y = 0;    point2d_vecPushBack(*points, p); // konec prvniho a zacatek druheho oblouku (nemenime nic)

	// U 2 RADKU NIZE ZMENIME ZNAMENKO U Y A DOPOCITAME X
	// X DOPOCITAME NAPRIKLAD TAK, ZE SI VEZMEME 2 OBECNE ROVNICE (y = kx + q) a ZA X a Y DOSADIME (DOSADIME HODNOTY 2 RIDICICH BODU, KTERE TOMU BODU PREDCHAZEJI)
	// VZNIKNOU NAM 2 ROVNICE: -250 = k * 160 + q (tohle jsou dosazene hodnoty ridicich bodu z radku 109)
	//						      0 = k * 200 + q (tohle jsou dosazene hodnoty ridicich bodu z radku 111)
	// DOPOCITAME k A q -> myslim, ze k vyslo 6,25 a q 1250 
	// DO OBECNE ROVNICE DOSADIME Y, k a q a ZJISTIME X ---> 180 = 6,25 (k) * X (neznama) + 1250 (q)
	// z tohohle nam vyjde prvni x = 228.8 (na radku 120)

	p.x = 228.8; p.y = 180; point2d_vecPushBack(*points, p); // Y s obracenym znamenkem; x jsme dopocitali
	p.x = 315; p.y = 180; point2d_vecPushBack(*points, p); // Y s obracenym znamenkem; x jsme dopocitali

	// radky nize nemenime
	p.x = 360; p.y = 0;    point2d_vecPushBack(*points, p);

	p.x = 390; p.y = -120;  point2d_vecPushBack(*points, p);
	p.x = 430; p.y = -120;  point2d_vecPushBack(*points, p);

	p.x = 460; p.y = 0;    point2d_vecPushBack(*points, p);

	// U TECHTO 2 RADKU NIZE ZMENIME ZNAMENKO U Y A DOPOCITAME X
	p.x = 477.5; p.y = 70;  point2d_vecPushBack(*points, p); // Y s obracenym znamenkem; x jsme dopocitali
	p.x = 517.5; p.y = 70;  point2d_vecPushBack(*points, p); // Y s obracenym znamenkem; x jsme dopocitali

	// radky nize nemenime
	p.x = 535; p.y = 0;    point2d_vecPushBack(*points, p);

	p.x = 545; p.y = -40;  point2d_vecPushBack(*points, p);
	p.x = 575; p.y = -40;  point2d_vecPushBack(*points, p);

	p.x = 585; p.y = 0;    point2d_vecPushBack(*points, p);

	Point2d offset = { static_cast<double>(offset_x), static_cast<double>(offset_y), 1.0 };
	for (int i = 0; i < (*points)->size; i++) {
		addPoint2d(point2d_vecGetPtr(*points, i), &offset, point2d_vecGetPtr(*points, i));
	}
	
}

/**
 * Implementace vypoctu Bezierove kubiky.
 * @param P1,P2,P3,P4 ridici body kubiky
 * @param precision pocet bodu na krivke, ktere chceme vypocitat
 * @param trajectory_points vystupni vektor bodu kubiky (nemazat, jen pridavat body)
 */
void bezierCubic(const Point2d* P1, const Point2d* P2, const Point2d* P3, const Point2d* P4,
	const int precision, S_Vector* trajectory_points) {

	/* == TODO ==
	 * Soucast Ulohy c.1:
	 * Sem pridejte kod vypoctu Bezierove kubiky. Body krivky pridavejte do trajectory_points.
	 */

	// tohle jsou veci z vzorce
	Point2d Q; // vytvorim si Q, ktere potrebuje na ten vzorec
	double B0, B1, B2, B3, t_change, t;

	// 1.0 nize je jak kdyby ten celek (100%) <-- neboli ta cela osa, na ktere to vykreslujeme a precision je pocet bodu, kolik tam chceme dat do tech krivek
	t_change = 1.0 / precision; // cim vetsi precision, tim vice bodu tam budeme mit a tim 'hezci' ta krivka (oblouk) bude

	// na tomto zaciname
	t = t_change;
	
	for (int i = 0; i <= precision; i++) {
		// B0,B1,B2,B3 jsou vsechny podle vzorce
		B0 = pow(1 - t, 3);
		B1 = 3*t*pow(1 - t, 2);
		B2 = 3*pow(t, 2) * (1 - t);
		B3 = pow(t, 3);

		// podle vzorce doplnime Q
		Q.x = P1->x * B0 + P2->x * B1 + P3->x * B2 + P4->x * B3;
		Q.y = P1->y * B0 + P2->y * B1 + P3->y * B2 + P4->y * B3;
		Q.weight = 1; // nevim jestli tam tohle musi byt, 1 je default

		point2d_vecPushBack(trajectory_points, Q);

		// pricteme, abychom se posunuli na dalsi bod
		t += t_change;
	}
}

/*
 * Implementace vypoctu trajektorie, ktera se sklada z Bezierovych kubik.
 * @param precision pocet bodu krivky, ktere mame urcit
 * @param control_points ridici body krivky
 * @param trajectory_points vystupni body zakrivene trajektorie
 */
void	bezierCubicsTrajectory(int precision, const S_Vector* control_points, S_Vector* trajectory_points) {
	// Toto musi byt na zacatku funkce, nemazat.
	point2d_vecClean(trajectory_points);

	/* == TODO ==
	 * Uloha c.1
	 * Ziskejte postupne 4 ridici body a pro kazdou ctverici vypocitejte body Bezierovy kubiky.
	 *
	 * Kostra:
	 *  for(...; ...; ...) {
	 *    Point2d *P1 = ...
	 *    Point2d *P2 = ...
	 *    Point2d *P3 = ...
	 *    Point2d *P4 = ...
	 *    bezierCubic(P1, P2, P3, P4, precision, trajectory_points);
	 *  }
	 * Nasledujici volani funkce getLinePoints(.) zmazte - je to jen ilustrace hranate trajektorie.
	 */

	// pricitame 3, jelikoz posledni bod jednoho oblouku je zaroven prvnim bodem toho nasledujiciho oblouku
	for (int i = 3; i < point2d_vecSize(control_points); i += 3) {
		// tohle jsou ty 4 ridici body kazdeho oblouku 
		Point2d* P1 = point2d_vecGetPtr(control_points, i - 3); // uplne prvni ridici bod daneho oblouku
		Point2d* P2 = point2d_vecGetPtr(control_points, i - 2); // druhy ridici bod daneho oblouku
		Point2d* P3 = point2d_vecGetPtr(control_points, i - 1); // treti ridici bod daneho oblouku
		Point2d* P4 = point2d_vecGetPtr(control_points, i); // ctvrty ridici bod daneho oblouku
		bezierCubic(P1, P2, P3, P4, precision, trajectory_points);
	}
}
