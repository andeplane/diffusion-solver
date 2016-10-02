#include "geometry.h"
#include "common.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

namespace Geometry {
    shared_ptr<Grid> linearGridX(int nx, int ny, int nz, int poreSize, real value1, real value2) {
        createTables();

        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz) );
        Grid &grid = *gridPtr;
        grid.iterate([&](real &value, short &cellPoreSize, int i, int , int ) {
            real factor = float(i) / (grid.nx()-1);
            value = factor * value2 + (1 - factor) * value1;
            cellPoreSize = poreSize;
            value = concentration(value, cellPoreSize);
        });

        return gridPtr;
    }

    shared_ptr<Grid> initialWallX(int nx, int ny, int nz, int poreSize, real value1, real value2) {
        createTables();

        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz) );
        Grid &grid = *gridPtr;
        grid.iterate([&](real &value, short &cellPoreSize, int i, int , int ) {
            if(i==0) {
                value = value1;
            } else {
                value = value2;
            }
            cellPoreSize = poreSize;
            value = concentration(value, cellPoreSize);
        });

        return gridPtr;
    }

    shared_ptr<Grid> cubeGridX(int nx, int ny, int nz, int poreSize1, int poreSize2, real value1, real value2, bool linear)
    {
        createTables();

        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz) );
        Grid &grid = *gridPtr;

        vector<bool> wallsX(grid.nx(),false);
        vector<bool> wallsY(grid.nx(),false);
        vector<bool> wallsZ(grid.nx(),false);

        for(int i=0; i<grid.nx(); i++) {
            double rnd = rand() / double(RAND_MAX);
            wallsX[i] = rnd < 0.1;
            wallsY[i] = rnd < 0.1;
            wallsZ[i] = rnd < 0.1;
        }

        grid.iterate([&](real &value, short &cellPoreSize, int i, int j, int k) {
            real factor = float(i) / (grid.nx()-1);
            value = factor * value2 + (1 - factor) * value1;
            if(!linear) value = 0;

            if(wallsX[i] || wallsY[j] || wallsZ[k]) {
                cellPoreSize = poreSize1;
            } else {
                cellPoreSize = rand() % 18 + 1;
            }
            value = concentration(value, cellPoreSize);
        });

        return gridPtr;
    }

    void updatePlaneGeometry(vector<bool> &isWall, vector<double> &poreSize, const vector<double> &planes, int N) {
        int wallThickness = 1;
        int wallCounter = 0;

        int planeIndex = -1;
        double nextPlane = 0;
        double L = planes[0];

        for(int i=0; i<N; i++) {
            if(i>=nextPlane) {
                planeIndex++;
                if(planeIndex >= planes.size()) planeIndex = 0;
                L = planes[planeIndex];
                nextPlane += L+wallThickness;
                wallCounter = 0;
            }

            if(wallCounter < wallThickness) {
                isWall[i] = true;
                poreSize[i] = L;
                wallCounter++;
                continue;
            } else {
                isWall[i] = false;
                poreSize[i] = L;
            }
        }
    }

    std::shared_ptr<Grid> planeGeometryGrid(int nx, int ny, int nz, string filename, real value1, real value2, bool linear)
    {
        createTables();

        auto gridPtr = make_shared<Grid>( Grid(nx, ny, nz) );
        Grid &grid = *gridPtr;
        ifstream file(filename.c_str());
        vector<double> planesX;
        vector<double> planesY;
        vector<double> planesZ;
        std::string line;
        int lineCount = 0;
        while (std::getline(file, line))
        {
            lineCount++;
            std::istringstream iss(line);
            if(line.find("#") == 0) continue; // Comment line, skip it
            if(lineCount == 2) continue; // Number of planes line, skip it
            double x,y,z;

            if (!(iss >> x >> y >> z)) {
                cout << "Error reading plane file " << filename << endl;
                break;
            } // error

            planesX.push_back(x);
            planesY.push_back(y);
            planesZ.push_back(z);
            cout << "Found walls " << x << ", " << y << ", " << z << endl;
        }
        cout << "Read file " << filename << endl;
        vector<bool> isWallX(nx, false);
        vector<bool> isWallY(ny, false);
        vector<bool> isWallZ(nz, false);

        vector<double> poreSizeX(nx, 0);
        vector<double> poreSizeY(ny, 0);
        vector<double> poreSizeZ(nz, 0);

        updatePlaneGeometry(isWallX, poreSizeX, planesX, nx);
        updatePlaneGeometry(isWallY, poreSizeY, planesY, ny);
        updatePlaneGeometry(isWallZ, poreSizeZ, planesZ, nz);

        for(int i=0; i<nx; i++) {
            double Lx = poreSizeX[i];
            for(int j=0; j<ny; j++) {
                double Ly = poreSizeY[j];
                for(int k=0; k<nz; k++) {
                    double Lz = poreSizeZ[k];

                    short &cellPoreSize = grid.poreSize(grid.index(i,j,k));
                    cellPoreSize = round(std::min(std::min(Lx, Ly), Lz));
                    if(isWallX[i] || isWallY[j] || isWallZ[k]) {
                        grid.isWall()[grid.index(i,j,k)] = true;
                    }

                    real &value = grid(i,j,k);
                    real factor = float(i) / (grid.nx()-1);
                    value = factor * value2 + (1 - factor) * value1;
                    if(!linear) value = 0;
                    value = concentration(value, cellPoreSize);
                }
            }
        }
        return gridPtr;
    }

}

