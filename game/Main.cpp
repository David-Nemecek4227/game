#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <chrono>
#include <thread>
#include <random>
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
int main()
{
    
    // create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Shoot the terrorist");
    sf::Clock clock;
    sf::Clock timer[3];
    sf::RectangleShape rects[3];
    sf::RectangleShape hp[3];

    for (int i = 0; i < 3; ++i) //vytvoření 3 objektů pro klikani
    {
        rects[i] = sf::RectangleShape(sf::Vector2f(75.f, 150.f));
        rects[i].setFillColor(sf::Color::Transparent);
        rects[i].setPosition(95.f + 280.f * i, 225.f);
    }
    for (int i = 0; i < 3; ++i) //vytvoření 3 objektů pro životy
    {
        hp[i] = sf::RectangleShape(sf::Vector2f(50.f, 50.f));
        hp[i].setFillColor(sf::Color::Red);
        hp[i].setPosition(10.f + 60.f * i, 10.f);
    }
    std::chrono::steady_clock::time_point lastClickTime[3] = { std::chrono::steady_clock::now(),std::chrono::steady_clock::now(),std::chrono::steady_clock::now() };
    sf::Texture bg; //vytvoření a načítaní pozadí
    bg.loadFromFile("res/pics/doors_bg.jpg"); 
    if (!bg.loadFromFile("res/pics/doors_bg.jpg")) { std::cout << "fail to load bg"; }
    sf::Sprite Sbg;
    Sbg.setTexture(bg);
    Sbg.setPosition(0, 100);

    sf::Texture txtrtgt;   //vytvoření a načítaní cílů 
    txtrtgt.loadFromFile("res/pics/target.png");
    if (!txtrtgt.loadFromFile("res/pics/target.png")) { std::cout << "fail to load target"; }

    sf::SoundBuffer bfr1; // vytvářeni sound bufferů pro zvuky
    if (!bfr1.loadFromFile("res/audio/ko.wav")) { std::cout << "fail to load sound"; }
    sf::Sound ko;
    ko.setBuffer(bfr1);
    
    sf::SoundBuffer bfr2;
    if (!bfr2.loadFromFile("res/audio/gunshot.wav")) { std::cout << "fail to load sound"; }
    sf::Sound gs;
    gs.setBuffer(bfr2);

    sf::SoundBuffer bfr3;
    if (!bfr3.loadFromFile("res/audio/blthit.wav")) { std::cout << "fail to load sound"; }
    sf::Sound bh;
    bh.setBuffer(bfr3);
    
    sf::SoundBuffer bfr4;
    if (!bfr4.loadFromFile("res/audio/scream.wav")) { std::cout << "fail to load sound"; }
    sf::Sound sc;
    sc.setBuffer(bfr4);
    
    int n = 0;
    int h = 3;
    int m = 0;
    int rg = 0;
  
    // game loop
    while (window.isOpen())
    {   
       
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)    //if podmínka pro kliknutí myší
            {
                
                if (event.mouseButton.button == sf::Mouse::Left)    //if podmínka pro kliknutí levým tlačítkem myši
                {
                    gs.play();
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));    //získání pozice kurzoru

                    for (int i = 0; i < 3; i++)
                    {
                        if (rects[i].getFillColor() == sf::Color::Red)  //kliknutí na červenou (nepřítel) a následné zmizení nepřítele a přičtení bodu
                        {
                            if (rects[i].getGlobalBounds().contains(mousePos))
                            {
                                
                                rects[i].setFillColor(sf::Color::Transparent);
                                lastClickTime[i] = std::chrono::steady_clock::now();
                                n++;
                           
                            }
                        }
                        if (rects[i].getFillColor() == sf::Color::Green)    //kliknutí na zelenou (rukojmí) a následné zmizení rukojmí a odečtení životu
                        {
                            if (rects[i].getGlobalBounds().contains(mousePos))
                            {
                                rects[i].setFillColor(sf::Color::Transparent);
                                lastClickTime[i] = std::chrono::steady_clock::now();
                                h--;
                                bh.play();
                                sc.play();

                            }
                        }
                    }
                }
            }
        }
        //čas na obnovení cílů
        auto now = std::chrono::steady_clock::now();
        std::srand(std::time(nullptr));
        if (clock.getElapsedTime().asSeconds() >= 2)    //časová prodleva aby se obrazce nezobrazovaly hned při spuštění
        {
            for (int i = 0; i < 3; i++)
            {
                if (rects[i].getFillColor() == sf::Color::Transparent  && now - lastClickTime[i] > std::chrono::seconds(std::rand() % 9 + 1))   //znovuzobrazení obrazců po uplynutí několika sekund
                {
                    rg=std::rand() % 9 + 1; //rozhodování zda zobrazit nepřítele nebo rukojmí
                    if (rg == 9)
                    {
                        rects[i].setFillColor(sf::Color::Green);    //rukojmí
                        rects[i].setTexture(&txtrtgt);
                        rects[i].setTextureRect(sf::IntRect(0, 0, 700, 1000));
                        timer[i].restart();
                    }
                    else if(6<rg && rg<9)
                    {
                        rects[i].setFillColor(sf::Color::Red);      //nepřítel 1
                        rects[i].setTexture(&txtrtgt);
                        rects[i].setTextureRect(sf::IntRect(700, 0, 700, 1000));
                        timer[i].restart();
                    }
                    else if (3 < rg && rg < 7)
                    {
                        rects[i].setFillColor(sf::Color::Red);      //nepřítel 2
                        rects[i].setTexture(&txtrtgt);
                        rects[i].setTextureRect(sf::IntRect(1400, 0, 700, 1000));
                        timer[i].restart();
                    }
                    else
                    {
                        rects[i].setFillColor(sf::Color::Red);      //nepřítel 3
                        rects[i].setTexture(&txtrtgt);
                        rects[i].setTextureRect(sf::IntRect(2100, 0, 700, 1000));
                        timer[i].restart();
                    }
                }
                
            }
            for (int i = 0; i < 3; i++)
            {
                if (rects[i].getFillColor() == sf::Color::Red && timer[i].getElapsedTime().asSeconds() > 2)     //akce po pozdním kliknutí na nepřítele
                {
                    timer[i].restart();
                    h--;
                    bh.play();
                }
            }
            for (int i = 0; i < 3; i++)
            {
                if (rects[i].getFillColor() == sf::Color::Green && timer[i].getElapsedTime().asSeconds() > 2)   //akce pro zmizení rukojmí pokud se na něj neklikne
                {
                    timer[i].restart();
                    rects[i].setFillColor(sf::Color::Transparent);
                }
            }
        }
        window.clear();     //vymazání okna
        window.draw(Sbg);   //vykreslení pozadí
        if (h > 0)
        {
            for (int i = 0; i < 3; i++) //vykreslení cílů
            {
                window.draw(rects[i]);
            }
        }    
        for (int i = 0; i < h; i++)     //vykreslení životů
        {
            window.draw(hp[i]);
        }
        //↓↓funkce pro zobrazení skore
        sf::Text text;
        sf::Font font;
        font.loadFromFile("res/fonts/font.ttf");
        if (h >= 0)
        {
            m = n;
        }
        else
        {
            m = m;
            bh.pause();
            gs.pause();
        }
        std::string s = std::to_string(m);
        text.setString(s);
        text.setFillColor(sf::Color::White);
        text.setCharacterSize(48);
        text.setFont(font);
        text.setPosition(sf::Vector2f(550.f, 550.f));
        text.setStyle(sf::Text::Bold);
        window.draw(text);
        //↑↑funkce pro zobrazení skore
        //↓↓funkce pro zobrazení game over textu
        sf::Text g_o;
        g_o.setString("Game over");
        g_o.setFillColor(sf::Color::White);
        g_o.setOutlineColor(sf::Color::Black);
        g_o.setOutlineThickness(4);
        g_o.setCharacterSize(128);
        g_o.setFont(font);
        g_o.setPosition(sf::Vector2f(175.f, 200.f));
        g_o.setStyle(sf::Text::Bold);
        if (h == 0)
        {
           ko.play();
        }
        if (h <= 0)
        {
            
            window.draw(g_o);
            
        }
        //↑↑funkce pro zobrazení game over textu
        window.display();
    }

    return 0;
}
