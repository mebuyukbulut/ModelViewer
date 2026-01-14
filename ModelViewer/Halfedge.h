#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>

namespace HE {
	class Halfedge;

	class Vertex {
	public:
		Vertex(glm::vec3 point = glm::vec3(0, 0, 0), Halfedge* edge = nullptr)
			: point{ point }, edge{ edge }
		{}

		glm::vec3 point;
		Halfedge* edge{}; // one outgoing halfedge from this vertex (origin)
		// consider an id for debug only
	};
	class Face {
	public:
		glm::vec3 normal;
		Halfedge* edge{}; // one halfedge on the boundary
	};
	class Halfedge
	{
	public:
		Halfedge* prev{};
		Halfedge* next{};
		Halfedge* twin{};
		Vertex* vertex{}; // origin vertex of this halfedge (v_from)
		Face* face{};
	};
	class Solid {
	public:
		std::vector<std::unique_ptr<Vertex>> vertices;
		std::vector<std::unique_ptr<Halfedge>> halfedges;
		std::vector<std::unique_ptr<Face>> faces;

		Vertex* addVertex(glm::vec3 point);
		Halfedge* createEdgePair(Vertex* v1, Vertex* v2);
		Face* createFace(std::vector<Vertex*> corners);
		Halfedge* findHalfedge(Vertex* a, Vertex* b);

		void bakeToRenderBuffers(std::vector<glm::vec3>& out_positions, std::vector<uint32_t>& out_indices);

	};

}

void HEmain() {
	HE::Solid mySolid;
	HE::Vertex* v0 = mySolid.addVertex(glm::vec3(0, 1, 0));
	HE::Vertex* v1 = mySolid.addVertex(glm::vec3(0, 0, 0));
	HE::Vertex* v2 = mySolid.addVertex(glm::vec3(1, 0, 0));
	HE::Vertex* v3 = mySolid.addVertex(glm::vec3(1, 1, 0));

	HE::Face* f0 = mySolid.createFace(std::vector<HE::Vertex*>{v0, v1, v2, v3});
}

HE::Vertex* HE::Solid::addVertex(glm::vec3 point) {
	auto p_v = std::make_unique<Vertex>();
	Vertex* v = p_v.get();
	vertices.push_back(std::move(p_v));

	v->point = point;

	return v;
}
HE::Halfedge* HE::Solid::createEdgePair(HE::Vertex* v1, HE::Vertex* v2) {

	// if already exists a->b, return it
	if (Halfedge* existing = findHalfedge(v1, v2)) return existing;

	//	 ---h1-->
	// v1		v2
	//  <--h2---
	auto p_h1 = std::make_unique<Halfedge>();
	auto p_h2 = std::make_unique<Halfedge>();
	Halfedge* h1 = p_h1.get();
	Halfedge* h2 = p_h2.get();
	halfedges.push_back(std::move(p_h1));
	halfedges.push_back(std::move(p_h2));


	h1->twin = h2;
	h2->twin = h1;

	h1->vertex = v1;
	h2->vertex = v2;

	if(!v1->edge) v1->edge = h1;
	if(!v2->edge) v2->edge = h2;

	return h1;
}
HE::Face* HE::Solid::createFace(std::vector<HE::Vertex*> corners) {
	if (corners.size() < 3) return nullptr;

	// compute signed area / orientation on plane (optional) to ensure CCW
	// TODO: handle degenerate case

	auto p_face = std::make_unique<Face>();
	Face* face = p_face.get();
	faces.push_back(std::move(p_face));

	// collect halfedges (creating pairs as needed)
	std::vector<Halfedge*> loop;
	loop.reserve(corners.size());


	for (size_t i = 0; i < corners.size(); ++i) {
		Vertex* a = corners[i];
		Vertex* b = corners[(i + 1) % corners.size()];

		Halfedge* he = findHalfedge(a, b);
		if (!he) {
			he = createEdgePair(a, b); // returns halfedge a->b with twin b->a
		}
		loop.push_back(he);
	}

	// link next/prev and assign face
	for (size_t i = 0; i < loop.size(); ++i) {
		Halfedge* h = loop[i];
		Halfedge* hn = loop[(i + 1) % loop.size()];
		Halfedge* hp = loop[(i + loop.size() - 1) % loop.size()];
		h->next = hn;
		h->prev = hp;
		h->face = face;
	}

	face->edge = loop[0];

	// compute normal using first 3 corners (ensure CCW)
	glm::vec3 a = corners[0]->point;
	glm::vec3 b = corners[1]->point;
	glm::vec3 c = corners[2]->point;
	face->normal = glm::normalize(glm::cross(b - a, c - a));

	return face;
}

HE::Halfedge* HE::Solid::findHalfedge(Vertex* a, Vertex* b) {
	// unordered_map<pair<Vertex*,Vertex*>, Halfedge*>
	for (auto& h_up : halfedges) {
		Halfedge* h = h_up.get();
		if (h->vertex != a) continue;
		if (h->next && h->next->vertex == b) return h; // h goes a -> b
		if (h->vertex == a && h->twin && h->twin->vertex == b) return h;
	}
	return nullptr;
}



void HE::Solid::bakeToRenderBuffers(std::vector<glm::vec3>& out_positions, std::vector<uint32_t>& out_indices){
	out_positions.clear();
	out_indices.clear();
	std::unordered_map<Vertex*, uint32_t> v_to_idx;
	v_to_idx.reserve(vertices.size() * 2);

	auto pushVertexIfNeeded = [&](Vertex* v) -> uint32_t {
		auto it = v_to_idx.find(v);
		if (it != v_to_idx.end()) return it->second;
		uint32_t idx = static_cast<uint32_t>(out_positions.size());
		out_positions.push_back(v->point);
		v_to_idx[v] = idx;
		return idx;
		};

	for (auto& f_up : faces) {
		Face* f = f_up.get();
		// collect loop vertices
		std::vector<Vertex*> loop;
		Halfedge* start = f->edge;
		if (!start) continue;

		Halfedge* h = start;
		do {
			loop.push_back(h->vertex);
			h = h->next;
			if (!h) break; // corrupted mesh
		} while (h != start);

		if (loop.size() < 3) continue;
		// triangle fan
		uint32_t i0 = pushVertexIfNeeded(loop[0]);
		for (size_t i = 1; i + 1 < loop.size(); ++i) {
			uint32_t i1 = pushVertexIfNeeded(loop[i]);
			uint32_t i2 = pushVertexIfNeeded(loop[i + 1]);
			out_indices.push_back(i0);
			out_indices.push_back(i1);
			out_indices.push_back(i2);
		}
	}
}
