#include "Euler.h"

int Face::size()
{
    if (FLoop == NULL)
        return 0;
    
    int count = 0;
    Loop* startL = FLoop;
    Loop* l = FLoop;
    do
    {
        count++;
        l = l->nextL;
    } while (l != NULL && l != startL);
    return count;
}

int Loop::size()
{
    if (HEs == NULL)
        return 0;

    int count = 0;
    HalfEdge* startHe = HEs;
    HalfEdge* he = HEs;
    do
    {
        count++;
        he = he->nextHE;
    } while (he != NULL && he != startHe);
    return count;
}

Solid* mvfs(Point p, Vertex*& v)
{
    // Create empty Solid, face and loop
    Solid* newS = new Solid();
    Face* newF = new Face();
    Loop* newL = new Loop();

    // Set vertex
    v = new Vertex();
    v->point = new Point();
    v->point->SetPoint(p);

    // Set ptrs
    newS->Faces = newF;
    newF->FSolid = newS;

    newF->FLoop = newL;
    newL->LFace = newF;

    return newS;
}

HalfEdge* mev(Vertex* v, Point p, Loop* l)
{
    // Create empty half edge, edge and vertex
    HalfEdge* he1 = new HalfEdge();
    HalfEdge* he2 = new HalfEdge();
    Edge* newEdge = new Edge();

    // Set ptrs between half edge
    newEdge->HE1 = he1;
    newEdge->HE2 = he2;
    he1->Edge = newEdge;
    he2->Edge = newEdge;
    he1->partner = he2;
    he2->partner = he1;

    // Set ptrs with vertex
    Vertex* newV = new Vertex();
    newV->point = new Point();
    newV->point->SetPoint(p);

    he1->startV = v;
    he1->endV = newV;
    he2->startV = newV;
    he2->endV = v;

    // Set ptrs with loop
    he1->HELoop = l;
    he2->HELoop = l;

    he1->nextHE = he2;
    he2->prevHE = he1;
    if (l->HEs == NULL)
    {
        he2->nextHE = he1;
        he1->prevHE = he2;
        l->HEs = he1;
    }
    else
    {
        HalfEdge* he;
        for (he = l->HEs; he->nextHE->startV != v; he = he->nextHE)
        {
        }
        he2->nextHE = he->nextHE;
        he->nextHE->prevHE = he2;

        he1->prevHE = he;
        he->nextHE = he1;
    }

    // Add edge to list
    Solid* S = l->LFace->FSolid;
    Edge* edges = S->Edges;
    if (edges == NULL)
    {
        S->Edges = newEdge;
    }
    else
    {
        while (edges->nextE != NULL)
        {
            edges = edges->nextE;
        }
        edges->nextE = newEdge;
        newEdge->prevE = edges;
    }
    return he1;
}

Loop* mef(Vertex* v1, Vertex* v2, Loop*& l)
{
    HalfEdge* he1 = new HalfEdge();
    HalfEdge* he2 = new HalfEdge();
    Edge* newE = new Edge();

    newE->HE1 = he1;
    newE->HE2 = he2;
    he1->Edge = newE;
    he2->Edge = newE;
    he1->partner = he2;
    he2->partner = he1;

    he1->startV = v1;
    he1->endV = v2;
    he2->startV = v2;
    he2->endV = v1;

    HalfEdge* he;
    HalfEdge* startHE;
    int Repeat;

    startHE = he = l->HEs;
    Repeat = 0;
    for (he = l->HEs; he->startV != v1; he = he->nextHE)
    {
        if (he == startHE && Repeat == 2)
        {
            cout << "mef Error" << endl;
            return NULL;
        }
        if (he == startHE && Repeat != 2)
        {
            Repeat++;
        }
    }
    HalfEdge* firstHE = he;

    startHE = he = l->HEs;
    Repeat = 0;
    for (he = l->HEs; he->startV != v2; he = he->nextHE)
    {
        if (he == startHE && Repeat == 2)
        {
            cout << "mef Error" << endl;
            return NULL;
        }
        if (he == startHE && Repeat != 2)
        {
            Repeat++;
        }
    }
    HalfEdge* secondHE = he;

    he2->nextHE = firstHE;
    he2->prevHE = secondHE->prevHE;
    he1->nextHE = secondHE;
    he1->prevHE = firstHE->prevHE;
    firstHE->prevHE->nextHE = he1;
    firstHE->prevHE = he2;
    secondHE->prevHE->nextHE = he2;
    secondHE->prevHE = he1;

    l->HEs = he1;
    Loop* newL = new Loop();
    newL->HEs = he2;

    Solid* S = l->LFace->FSolid;

    Face* newF = new Face();
    newF->FLoop = newL;
    newL->LFace = newF;
    newF->FSolid = S;

    Face* f;
    for (f = S->Faces; f->nextF != NULL; f = f->nextF)
    {
    }
    f->nextF = newF;
    newF->prevF = f;

    Edge* edges = S->Edges;
    if (NULL == edges)
    {
        S->Edges = newE;
    }
    else
    {
        while (edges->nextE != NULL)
        {
            edges = edges->nextE;
        }
        edges->nextE = newE;
        newE->prevE = edges;
    }
    if (newL->size() > l->size())
    {
        Loop* temp = newL;
        newL = l;
        l = temp;
    }
    return newL;
}

Loop* kemr(Vertex* v1, Vertex* v2, Loop*& l)
{
    Loop* newL = new Loop();

    HalfEdge* he;
    for (he = l->HEs; !(v1 == he->startV && v2 == he->endV); he = he->nextHE)
    {
    }

    Edge* e = he->Edge;
    he->nextHE->prevHE = he->partner->prevHE;
    he->partner->prevHE->nextHE = he->nextHE;

    he->prevHE->nextHE = he->partner->nextHE;
    he->partner->nextHE->prevHE = he->prevHE;

    l->HEs = he->prevHE;
    newL->HEs = he->nextHE;

    Face* F = l->LFace;
    newL->LFace = F;

    Loop* lp;
    for (lp = F->FLoop; NULL != lp->nextL; lp = lp->nextL);
    lp->nextL = newL;
    newL->prevL = lp;

    Solid* S = l->LFace->FSolid;
    e = S->Edges;
    if (e == he->Edge)
    {
        S->Edges = he->Edge->nextE;
        S->Edges->prevE = e->prevE;
    }
    else
    {
        while (e->nextE != he->Edge)
        {
            e = e->nextE;
        }
        e->nextE = he->Edge->nextE;
        e->nextE->prevE = e;
    }
    if (newL->size() > l->size())
    {
        Loop* temp = newL;
        newL = l;
        l = temp;
    }
    return newL;
}

void kfmrh(Loop* outL, Loop* innerL)
{
    Face* f1 = outL->LFace;
    Face* f2 = innerL->LFace;

    Loop* lp;
    for (lp = f1->FLoop; NULL != lp->nextL; lp = lp->nextL)
    {
    }
    lp->nextL = innerL;
    innerL->prevL = lp;
    innerL->LFace = f1;

    Solid* S = f1->FSolid;
    Face* f = S->Faces;
    if (f == f2)
    {
        S->Faces = f->nextF;
        S->Faces->prevF = f->prevF;
    }
    else
    {
        while (f->nextF != f2)
        {
            f = f->nextF;
        }
        f->nextF = f2->nextF;
        f->nextF->prevF = f;
    }
    return;
}

Solid* sweep(Face* f, glm::vec3 direction, float distance)
{
    Solid* S = f->FSolid;
    Loop* loop;
    HalfEdge* he;
    for (loop = f->FLoop; loop != NULL; loop = loop->nextL)
    {
        he = loop->HEs;
        Vertex* startV = he->startV;
        glm::vec3 newPosition = startV->point->p + distance * direction;
        Point newPoint(newPosition.x, newPosition.y, newPosition.z);

        HalfEdge* firstHE = mev(startV, newPoint, loop);
        Vertex* upVertex_1 = firstHE->endV;

        he = he->nextHE;
        Vertex* v = he->startV;
        while (v != startV)
        {
            glm::vec3 newPosition = v->point->p + distance * direction;
            Point newPoint(newPosition.x, newPosition.y, newPosition.z);

            HalfEdge* connectHe = mev(v, newPoint, loop);
            Vertex* upVertex_2 = connectHe->endV;
            mef(upVertex_2, upVertex_1, loop);
            upVertex_1 = upVertex_2;
            he = he->nextHE;
            v = he->startV;
        }
        mef(firstHE->endV, upVertex_1, loop);
    }
    return S;
}


