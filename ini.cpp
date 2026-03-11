#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include< stdlib.h >

class Ini {

private:
    std::vector<std::string> lines;

    std::string removeedgespaces(std::string str)
    {
        size_t strBegin;
        size_t strEnd;
        strBegin = str.find_first_not_of(' ');
        strEnd = str.find_last_not_of(' ');
        str.erase(strEnd + 1, str.size() - strEnd);
        str.erase(0, strBegin);
        return str;
    }

    std::string substr(std::string str, size_t start, size_t end)
    {
        std::string tmp = "";
        if ((start > str.size()) or (end > str.size())) return "error substr";
        for (int i = start; i < end; i++)
        {
            tmp = tmp + str[i];
        }
        return tmp;
    }

public:
    int linecounter = 0; //число строк в файле и в массиве

 

    void start(std::string filename) //открываем файл и читаем в массив содержимое
    {
        try
        {
            std::string v;
            std::ifstream ifs(filename);
            std::string g2;
            int it2;


            if (!ifs.is_open())
            {
                throw "file error";
                //std::cout << "fileerror" << std::endl;
                //тут будет исключение при ошибке с файлом

            }
            while (std::getline(ifs, v))
            {
                if (std::size(v) > 2) //меньше двух символов в строке не берем
                {
                    //отрезаем всё после точки с запятой
                    it2 = v.find_first_of(';', 0);
                    if (it2 > 0) g2.resize(it2);
                    if (it2 > 0)
                    {

                        //std::string g2(it2, '\0');
                        std::copy(v.begin(), v.begin() + v.find_first_of(';'), g2.begin());
                        // std::cout << "отрезаем точку с запятой: !" << g2 << "!" << std::endl;

                    }
                    else g2 = v;

                    //убираем пробелы в начале и в конце
                    g2 = removeedgespaces(g2);
                    //std::cout << "убираем пробелы: !" << g2 << "!" << std::endl;
                    if (std::size(g2) > 2) //если осталось что-то годное, добавляем в список
                    {
                        lines.push_back(g2);
                        linecounter++;
                    }

                }
            }

            ifs.close();
        }//try
        catch (const char* error_message)
        { 
            std::cout << error_message << std::endl;
        }
    }//void start


private:
 
std::string get(std::string section, std::string variable)
    {
        section = "[" + section + "]";
        
        std::string answer ;
        std::string g2;
        int it2;
        bool foundsection = false, foundvariable = false; //флаги успеха в поиске
        bool tmpflagsection = true, tmpflagvariable = true;

        //мотаем до нужной секции конфигурации
        int i = 0;
        while (i < linecounter)
        {
            
            if (section == lines[i]) //нашли секцию
            {
                foundsection = true;
                tmpflagvariable = true;
                i++;
                while (tmpflagvariable)
                {

                    it2 = lines[i].find_first_of('=', 0);
                    //            if (it2 > 0) g2.resize(it2);
                    g2 = substr(lines[i], 0, it2);
                    if (g2 == variable) //нашли параметр
                    {
                        it2 = lines[i].find_first_of('=');


                        if (it2 > 0)
                        {
                            g2 = substr(lines[i], it2 + 1, lines[i].size());
                            answer =removeedgespaces(g2);
                      
                         // std::string str = removeedgespaces(g2);
                         //   answer = atoi(str.c_str());

                            foundvariable = true;
                        }
                    }


                    if ((lines[i][0] == '[') or (i >= linecounter - 1)) //началась другая секция либо закончился список 
                    {
                        tmpflagvariable = false;
                    }

                    if (tmpflagvariable) i++;
                }//while поиск переменной

                if (i >= linecounter - 1) //закончился список 
                {
                    tmpflagsection = false;
                }
                if (tmpflagsection) i++;
            }//конец нашли секцию

            i++;
        }//while поиск секции

        //конвертируем


        
        return answer;
    }//get


public:
template<typename T>
T get_value(const std::string& section, const std::string& variable) 
{ 
    static_assert(sizeof(T) == -1, "not implemented type for get_value"); 
} 

};

//для стринга
template<>
std::string Ini::get_value(const std::string& section, const std::string& variable) {
    return get(section, variable);
}
//для инта
template<>
int Ini::get_value(const std::string& section, const std::string& variable) 
{
    std::string s;
    s=get(section, variable);
    return std::stoi(s);
}
//для дабла
template<>
double Ini::get_value(const std::string& section, const std::string& variable) 
{
    std::string s;
    s = get(section, variable);
    return std::stod(s);
}


    






int main()
{
    setlocale(LC_ALL, "Russian");

    Ini myconf; //создаем объект
    myconf.start("config.ini");


    std::cout << "str: " << myconf.get_value<std::string>("General", "language") << std::endl;
    std::cout << "int: "<< myconf.get_value<int>("Position", "x") << std::endl;
    std::cout << "double: "<< myconf.get_value<double>("General", "some") << std::endl;



}
