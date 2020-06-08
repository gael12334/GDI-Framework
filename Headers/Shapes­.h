#ifndef SHAPES_H__
#define SHAPES_H__

#include "Bitmap.h"

namespace GDI {
	typedef struct PointList : public Point {
		PointList* next;

		PointList(int x, int y, PointList* next = nullptr) : Point(x, y) {
			this->next = next;
		}

		PointList& operator,(PointList p) {
			this->next = new PointList(p.x, p.y, p.next);
		}

		static uint Count(PointList* pl) {
			if (pl) {
				if (pl->next)
					return 1 + Count(pl->next);
				return 1;
			}
		}

		static void Delete(PointList* pl) {
			if (pl) {
				if(pl->next)
					Delete(pl->next);
				delete pl;
				pl = nullptr;
			}
		}

	} parg;

	struct Line {
		Color color;
		Point* vertices;
		uint count;

		Line(Color color, parg point) {
			this->color = color;
			this->count = point.Count(&point);
			this->vertices = new Point[this->count];

			parg* it = &point;
			uint c = 0;
			while (it) {
				vertices[c] = { it->x, it->y };
				c++;
				it = it->next;
			}

			parg::Delete(it);
		}

		~Line() {
			delete[] vertices;
		}
	};
}

#endif
