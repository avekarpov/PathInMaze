#include <maze.h>
#include <fstream>

Maze maze;
bool is_size = false;

void menu(std::string msg = "")
{
    system("clear");

    if(msg != "")
    {
        std::cout << msg << std::endl << std::endl;
    }

    std::cout << "open – Открыть сохранение лабиринта" << std::endl;
    std::cout << "resize - Задать размер лабиринта" << std::endl;
    if(::is_size)
    {
        std::cout << "size - размер лабиринта" << std::endl;
        std::cout << "generate - сгенерировать лабиринт" << std::endl;
        std::cout << "draw - отрисовать лабиринт" << std::endl;
        std::cout << "path - поиск пути в лабиринте" << std::endl;
        std::cout << "save – сохранить лабиринт" << std::endl;
    }
    else
    {
        std::cout << "Размер лабиринта не задан! Получить размер невозможно" << std::endl;
        std::cout << "Размер лабиринта не задан! Генерация лабиринта невозможна" << std::endl;
        std::cout << "Размер лабиринта не задан! Отрисовка лабиринта невозможна" << std::endl;
        std::cout << "Размер лабиринта не задан! Поиск пути в лабиринте невозможен" << std::endl;
        std::cout << "Размер лабиринта не задан! Сохранение лабиринта невозможно" << std::endl;
    }
    std::cout << "exit - выход из программы" << std::endl;
    std::cout << std::endl;
}

void run()
{
    std::string command;
    std::string msg;

    do{
        do
        {
            ::menu(msg);
            std::cin >> command;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

            msg = "команад " + command + " не найдена";
        }
        while((command != "open") &&
              (command != "resize") && (command != "size")  &&
              (command != "generate") &&
              (command != "draw") && (command != "path") &&
              (command != "save") &&
              (command != "exit"));

        if(command == "open")
        {
            std::string fpath;
            std::ifstream fin;

            Maze buf_maze;
            Point size;


            fin.exceptions(std::ifstream::badbit | std::ifstream::failbit);

            try
            {
                system("clear");
                std::cout << "Введите путь до файла сохранения: ";
                std::cin >> fpath;

                fin.open(fpath);

                fin >> size;
                ::maze.resize(size);
                fin >> maze;

                ::is_size = true;

                msg = "Лабиринт открыт";
            }
            catch(std::exception& ex)
            {
                ::maze.resize(NULLPOINT);

                msg = "При открытии файла возникла ошибка: ";
                msg += ex.what();
                msg += "\nЛабиринт обнулен";
            }
            catch(Maze::Area::size_error& ex)
            {
                msg = "Размер " + Point::to_string(ex.what_size()) + " недопустим";
            }
        }

        else if(command == "resize")
        {
            Point size;

            system("clear");
            std::cout << "Введите размер лабиринта по (x, y): ";
            std::cin >> size;

            try
            {

                ::maze.resize(size);
                ::is_size = true;

                msg = "Размер задан";
            }
            catch(Maze::Area::size_error& ex)
            {
                msg = "Размер " + Point::to_string(ex.what_size()) + " недопустим";
            }
        }

        else if(command == "size")
        {
            if(!::is_size)
            {
                msg = "Размер не задан";
            }
            else
            {
                msg = Point::to_string(::maze.size());
            }
        }

        else if(command == "generate")
        {
            if(!::is_size)
            {
                msg = "Генерация невозможна";
            }
            else
            {
                try
                {
                    ::maze.generate();
                    msg = "Лабиринт сгененрирован";
                }
                catch(Maze::Area::out_of_area_error)
                {
                    msg = "Недопустимый размер лабиринта для генерации";
                }
            }
        }

        else if(command == "draw")
        {
            if(!::is_size)
            {
                msg = "Отрисовка лабиринта невозможна";
            }
            else
            {
                ::maze.draw();
                getchar();

                msg = "";
            }
        }

        else if(command == "path")
        {
            if(!::is_size)
            {
                msg = "Поиск пути невозможен";
            }
            else
            {
                system("clear");

                Point begin, end;
                Path path;
                char ans;

                std::cout << "Введите координты" << std::endl;
                std::cout << "\tНачала: ";
                std::cin >> begin;
                std::cout << "\tКонца:  ";
                std::cin >> end;

                try
                {
                    path = ::maze.path(begin, end);

                    if(path.size() > 0)
                    {
                        do
                        {
                            system("clear");
                            std::cout << "Анимировать путь? [Y/N]: ";
                            std::cin >> ans;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
                            ans = std::toupper(ans);
                        }
                        while((ans != 'Y') && (ans != 'N'));

                        if(ans == 'Y')
                        {
                            for(unsigned long int i = 1; i < path.size() - 1; ++i)
                            {
                                ::maze.draw(path, i);
                            }
                        }

                        ::maze.draw(path, path.size());
                        getchar();

                        msg = "";
                    }
                    else
                    {
                        msg = "Путь не найден";
                    }
                }
                catch(Maze::Area::out_of_area_error& ex)
                {
                    msg = "Точка " + Point::to_string(ex.what_point()) + " вне лабиринта";
                }
            }
        }

        else if(command == "save")
        {
            if(!::is_size)
            {
                msg = "Сохранение невозможно";
            }
            else
            {
                std::string fpath;
                std::ofstream fout;


                try
                {
                    system("clear");
                    std::cout << "Введите путь до файла сохранения: ";
                    std::cin >> fpath;

                    fout.open(fpath);

                    fout << ::maze.size() << std::endl;
                    fout << ::maze;

                    msg = "Лабиринт сохранен";
                }
                catch(std::exception& ex)
                {
                    msg = "При сохранении возникла ошика: ";
                    msg += ex.what();
                }
            }
        }
    }
    while(command != "exit");
}

int main()
{
    setlocale(LC_ALL, "Rus");

    ::run();

    return 0;
}
