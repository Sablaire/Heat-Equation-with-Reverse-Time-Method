#include <iostream>
#include <math.h>
#include <fstream>
#define nx 80 // Grid size in X direction
#define ny 80 // Grid size in Y direction

using namespace std;

int main() {
    // Create file for output
    ofstream fout("uravnenie.txt");

    // Declare variables and constants
    double u[ny], new_u[ny], q[ny], fi[ny], new_fi[ny], m[ny], f[ny], J;
    double v[ny], alphaM, new_v[ny], dx, dy, exit = 1e-5, max, A = 0.1, gradJ[ny], s1, s2, dt, T, pi = 3.14159285;
    int i, j, k, M, n = 10;

    // Initialize grid and boundary conditions
    dx = 1.0 / (nx - 1);
    dy = 1.0 / (ny - 1);
    dt = 0.5 * pow(dy, 2.0);
    T = 1000 * dt;
    u[0] = 0; u[ny - 1] = 0;
    new_u[0] = 0; new_u[ny - 1] = 0;
    v[0] = 0.0; new_v[ny - 1] = 0.0;
    fi[0] = 0.0; new_fi[ny - 1] = 0.0;

    // Set initial values
    for (j = 0; j < ny; j++) {
        q[j] = 0.1;
        f[j] = (1.0 / n) * sin(n * j * dy * pi);
    }

    M = 0;
    for (;;) {
        M++;
        for (int z = 0; z <= 1000; z++) {
            for (j = 1; j < ny - 1; j++) {
                u[j] = q[j];
                new_u[j] = q[j];
            }
            u[0] = 0.0; u[ny - 1] = 0.0;
            new_u[0] = 0.0; new_u[ny - 1] = 0.0;
            for (j = 1; j < ny - 1; j++) {
                u[j] = dt * (new_u[j + 1] - 2 * new_u[j] + new_u[j - 1]) / pow(dy, 2) + new_u[j];
            }
            for (j = 0; j < ny; j++)
                new_u[j] = u[j];
        }

        // Calculate the objective function
        for (j = 0; j < ny; j++)
            m[j] = u[j] - f[j];
        J = 0;
        for (j = 0; j < ny - 1; j++)
            J += (pow(m[j + 1], 2) + pow(m[j], 2)) * dy / 2;
        cout << J << "\tstep" << M << "\n";

        for (int z = 0; z <= 1000; z++) {
            fi[0] = 0.0; new_fi[0] = 0.0;
            new_fi[ny - 1] = 0.0; fi[ny - 1] = 0.0;
            for (j = 1; j < ny - 1; j++) {
                new_fi[j] = 2 * m[j];
                fi[j] = 2 * m[j];
            }
            for (j = 1; j < ny - 1; j++) {
                new_fi[j] = dt * (fi[j + 1] - 2 * fi[j] + fi[j - 1]) / pow(dy, 2) + fi[j];
            }
            for (j = 0; j < ny; j++)
                fi[j] = new_fi[j];
        }

        for (j = 0; j < ny; j++)
            gradJ[j] = fi[j];

        for (int z = 0; z <= 1000; z++) {
            v[0] = 0.0; new_v[0] = 0.0;
            v[ny - 1] = 0.0; new_v[ny - 1] = 0.0;
            for (j = 1; j < ny - 1; j++) {
                v[j] = fi[j];
                new_v[j] = fi[j];
            }
            for (j = 1; j < ny - 1; j++) {
                v[j] = dt * (new_v[j + 1] - 2 * new_v[j] + new_v[j - 1]) / pow(dy, 2) + new_v[j];
            }
            for (j = 0; j < ny; j++)
                new_v[j] = v[j];
        }

        s1 = 0; s2 = 0;
        for (j = 0; j < ny; j++) {
            s1 += pow(gradJ[j], 2);
            s2 += pow(v[j], 2);
        }
        alphaM = s1 / (2 * s2);
        cout << alphaM << endl;

        for (j = 0; j < ny; j++)
            q[j] = q[j] - alphaM * gradJ[j];
    }

    // Save results to file
    fout << "VARIABLES=\"x\", \"u\"" << endl;
    fout << "ZONE I=" << ny << ", F=POINT" << endl;
    for (j = 0; j < ny; j++) {
        fout << j * dy << "\t" << u[j] << endl;
    }

    fout << "VARIABLES=\"y\", \"f\", \"gradJ\", \"q\"" << endl;
    fout << "ZONE J=" << ny << ", F=POINT" << endl;
    for (j = 0; j < ny; j++) {
        fout << j * dy << "\t" << f[j] << "\t" << gradJ[j] << "\t" << q[j] << endl;
    }

    return 0;
}
