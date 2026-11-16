
#include <iostream>
#include <windows.h>
#include <fstream>
using namespace std;

void gotoxy(int x, int y)
{
    COORD coordinate;
    coordinate.X = x;
    coordinate.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

struct Point
{
    int x;
    int y;

    Point() : x(0), y(0) {}

    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    }
};

// SHAPE
class Shape
{
protected:
    string type;

public:
    Shape(string type) : type(type) {}

    virtual void Print() const = 0;
    virtual void Save() const = 0;

    virtual ~Shape() {}
};

// LINE
class Line : public Shape
{
private:
    Point start;
    Point end;

public:
    Line() : Shape("Line") {}

    Line(Point start, Point end) : Shape("Line")
    {
        this->start = start;
        this->end = end;
    }

    void Print() const override
    {
        int dx = end.x - start.x;
        int dy = end.y - start.y;

        int steps = abs(dx);
        if (abs(dy) > steps)
            steps = abs(dy);

        if (steps == 0)
        {
            gotoxy(start.x, start.y);
            cout << "*";
            return;
        }

        for (int i = 0; i <= steps; i++)
        {
            int x = start.x + dx * i / steps;
            int y = start.y + dy * i / steps;

            gotoxy(x, y);
            cout << "*";
        }
    }

    void Save() const override
    {
        ofstream file("shapes.txt", ios::app);

        file << "Type: " << type << endl;
        file << "Start: " << start.x << " " << start.y << endl;
        file << "End: " << end.x << " " << end.y << endl;
        file << endl;

        file.close();
    }
};

// RECTANGLE
class Rectangle : public Shape
{
private:
    Point topLeft;
    int dovgina;
    int shurina;

public:
    Rectangle() : Shape("Rectangle")
    {
        dovgina = 0;
        shurina = 0;
    }

    Rectangle(Point p, int dovgina, int shurina)
        : Shape("Rectangle")
    {
        topLeft = p;
        this->dovgina = dovgina;
        this->shurina = shurina;
    }

    void Print() const override
    {
        for (int i = 0; i < shurina; i++)
        {
            for (int j = 0; j < dovgina; j++)
            {
                if (i == 0 || i == shurina - 1 ||
                    j == 0 || j == dovgina - 1)
                {
                    gotoxy(topLeft.x + j, topLeft.y + i);
                    cout << "*";
                }
            }
        }
    }

    void Save() const override
    {
        ofstream file("shapes.txt", ios::app);

        file << "Type: " << type << endl;
        file << "Top left: " << topLeft.x << " "
            << topLeft.y << endl;
        file << "Dovgina: " << dovgina << endl;
        file << "Shurina: " << shurina << endl;
        file << endl;

        file.close();
    }
};

// POLYLINE
class Polyline : public Shape
{
private:
    Point* point;
    int countPoints;

public:
    Polyline() : Shape("Polyline")
    {
        point = nullptr;
        countPoints = 0;
    }

    void AddNewPoint(Point p)
    {
        Point* temp = new Point[countPoints + 1];

        for (int i = 0; i < countPoints; i++)
        {
            temp[i] = point[i];
        }

        temp[countPoints] = p;

        if (point != nullptr)
            delete[] point;

        point = temp;
        countPoints++;
    }

    void Print() const override
    {
        for (int i = 0; i < countPoints; i++)
        {
            gotoxy(point[i].x, point[i].y);
            cout << "*";
        }
    }

    void Save() const override
    {
        ofstream file("shapes.txt", ios::app);

        file << "Type: " << type << endl;

        for (int i = 0; i < countPoints; i++)
        {
            file << "Point " << i + 1 << ": "
                << point[i].x << " "
                << point[i].y << endl;
        }

        file << endl;
        file.close();
    }

    ~Polyline()
    {
        delete[] point;
    }
};

int main()
{
    Line line(Point(5, 5), Point(25, 15));

    Rectangle rectangle(Point(30, 5), 15, 8);

    Polyline polyline;

    polyline.AddNewPoint(Point(5, 20));
    polyline.AddNewPoint(Point(15, 25));
    polyline.AddNewPoint(Point(25, 20));
    polyline.AddNewPoint(Point(35, 25));

    line.Print();
    rectangle.Print();
    polyline.Print();

    line.Save();
    rectangle.Save();
    polyline.Save();

    gotoxy(0, 35);
    cout << "All shapes saved!";

    return 0;
}
