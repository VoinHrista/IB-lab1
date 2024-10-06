#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <algorithm>


using namespace std;

// Функция для генерации случайного квадрата 5x5
vector<vector<char>> generateSquare() {
    vector<char> alphabet;
    for (char c = 'A'; c <= 'Z'; ++c) {
        if (c != 'J') // исключаем 'J', так как 'I' и 'J' считаются одной буквой
            alphabet.push_back(c);
    }

    // Тасуем алфавит
    random_shuffle(alphabet.begin(), alphabet.end());

    // Создаем квадрат 5x5
    vector<vector<char>> square(5, vector<char>(5));
    int index = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            square[i][j] = alphabet[index++];
        }
    }
    return square;
}

// Функция для поиска позиции символа в квадрате
pair<int, int> findPosition(char letter, vector<vector<char>>& square) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (square[i][j] == letter)
                return {i, j};
        }
    }
    return {-1, -1}; // если не найдено
}

// Функция для шифрования биграммы
string encryptBigram(char first, char second, vector<vector<char>>& square1, vector<vector<char>>& square2) {
    pair<int, int> pos1 = findPosition(first, square1);
    pair<int, int> pos2 = findPosition(second, square2);

    // Формируем зашифрованную биграмму, согласно правилам двойного квадрата
    return string(1, square1[pos1.first][pos2.second]) + string(1, square2[pos2.first][pos1.second]);
}

// Функция для дешифрования биграммы
string decryptBigram(char first, char second, vector<vector<char>>& square1, vector<vector<char>>& square2) {
    pair<int, int> pos1 = findPosition(first, square1);
    pair<int, int> pos2 = findPosition(second, square2);

    // Формируем расшифрованную биграмму
    return string(1, square1[pos1.first][pos2.second]) + string(1, square2[pos2.first][pos1.second]);
}

// Функция для подготовки текста (убираем пробелы и приводим к верхнему регистру)
string prepareText(const string& text) {
    //cout << text << endl;
    string result;
    for (char c : text) {
        if (isalpha(c)) {
            result += toupper(c);
        }
    }
    // Дополняем до четного количества символов, если текст нечетный
    if (result.size() % 2 != 0)
        result += 'X';
    //cout << result << endl;
    return result;
}

// Функция для шифрования текста
string encryptText(const string& text, vector<vector<char>>& square1, vector<vector<char>>& square2) {
    string preparedText = prepareText(text);
    string encryptedText;
    for (size_t i = 0; i < preparedText.length(); i += 2) {
        encryptedText += encryptBigram(preparedText[i], preparedText[i + 1], square1, square2);
    }
    return encryptedText;
}

// Функция для дешифрования текста
string decryptText(const string& encryptedText, vector<vector<char>>& square1, vector<vector<char>>& square2) {
    string decryptedText;
    for (size_t i = 0; i < encryptedText.length(); i += 2) {
        decryptedText += decryptBigram(encryptedText[i], encryptedText[i + 1], square1, square2);
    }
    return decryptedText;
}

// Функция для чтения файла
string readFile(const string& filename) {
    ifstream file(filename);
    string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();
    return content;
}

// Функция для записи в файл
void writeFile(const string& filename, const string& content) {
    ofstream file(filename);
    file << content;
    file.close();
}

int main() {
    srand(time(0)); // Инициализация генератора случайных чисел
    setlocale(LC_ALL, "Russian");

    // Генерация двух квадратов
    vector<vector<char>> square1 = generateSquare();
    vector<vector<char>> square2 = generateSquare();

    cout << "First square: \n";
    for (int i = 0; i < square1.size(); i++){
        for (int j = 0; j < square1.size(); j++){
            cout << square1[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\nSecond square: \n";
    for (int i = 0; i < square2.size(); i++){
        for (int j = 0; j < square2.size(); j++){
            cout << square2[i][j] << " ";
        }
        cout << endl;
    }

    // Чтение исходного файла
    string inputFilename = "input.txt";
    string encryptedFilename = "encrypted.txt";
    string decryptedFilename = "decrypted.txt";

    string text = readFile(inputFilename);

    // Шифрование
    string encryptedText = encryptText(text, square1, square2);
    writeFile(encryptedFilename, encryptedText);

    // Дешифрование
    string decryptedText = decryptText(encryptedText, square1, square2);
    writeFile(decryptedFilename, decryptedText);

    cout << "Input text: " << text << endl;
    cout << "Encryptet text: " << encryptedText << endl;
    cout << "Decryptet text: " << decryptedText << endl;

    return 0;
}
