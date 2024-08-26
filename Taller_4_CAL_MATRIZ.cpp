
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Función para crear una matriz dinámica
int** createMatrix(int rows, int cols) {
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; i++) {
        matrix[i] = new int[cols];
    }
    return matrix;
}

// Función para liberar la memoria de una matriz dinámica
void deleteMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

// Función para guardar una matriz en un archivo de texto
void saveMatrixToTextFile(const string& filename, int** matrix, int rows, int cols) {
    ofstream outFile(filename);
    if (!outFile) {
        cerr << "No se pudo abrir el archivo para escribir." << endl;
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            outFile << matrix[i][j] << " ";
        }
        outFile << endl;
    }
    outFile.close();
}

// Función para leer una matriz desde un archivo de texto
int** loadMatrixFromTextFile(const string& filename, int& rows, int& cols) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "No se pudo abrir el archivo para leer." << endl;
        return nullptr;
    }

    vector<vector<int>> tempMatrix;
    string line;
    while (getline(inFile, line)) {
        istringstream iss(line);
        vector<int> row;
        int value;
        while (iss >> value) {
            row.push_back(value);
        }
        tempMatrix.push_back(row);
    }
    inFile.close();

    rows = tempMatrix.size();
    cols = tempMatrix[0].size();
    
    int** matrix = createMatrix(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = tempMatrix[i][j];
        }
    }
    return matrix;
}

// Función para escribir una matriz en un archivo binario
void writeBinaryFile(const string& filename, int** matrix, int rows, int cols) {
    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "No se pudo abrir el archivo para escribir." << endl;
        return;
    }
    for (int i = 0; i < rows; ++i) {
        outFile.write(reinterpret_cast<const char*>(matrix[i]), cols * sizeof(int));
    }
    outFile.close();
}

// Función para leer una matriz desde un archivo binario
void readBinaryFile(const string& filename, int** matrix, int rows, int cols) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "No se pudo abrir el archivo para leer." << endl;
        return;
    }
    for (int i = 0; i < rows; ++i) {
        inFile.read(reinterpret_cast<char*>(matrix[i]), cols * sizeof(int));
    }
    inFile.close();
}

// Función para mostrar una matriz en la consola
void displayMatrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int rows, cols;
    
    // Leer la matriz desde el archivo de texto
    int** matrix = loadMatrixFromTextFile("resultados.txt", rows, cols);

    if (matrix == nullptr) {
        cerr << "Error al cargar la matriz desde el archivo de texto." << endl;
        return 1;
    }
    
    // Guardar la matriz en un archivo binario
    writeBinaryFile("resultados.bin", matrix, rows, cols);
    
    // Crear una nueva matriz para cargar los datos desde el archivo binario
    int** loadedMatrix = createMatrix(rows, cols);
    
    // Leer la matriz desde el archivo binario
    readBinaryFile("resultados.bin", loadedMatrix, rows, cols);
    
    // Mostrar la matriz cargada desde el archivo binario
    cout << "Matriz leída desde el archivo binario:" << endl;
    displayMatrix(loadedMatrix, rows, cols);
    
    // Liberar la memoria
    deleteMatrix(matrix, rows);
    deleteMatrix(loadedMatrix, rows);
    
    return 0;
}
