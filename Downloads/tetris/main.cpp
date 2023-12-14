#include "Tetris.h"
#include "TextBox.h"
using namespace sf;

//login
struct scn2
{
    bool email = false;
    bool password = false;
};

struct scn3
{
    bool email = false;
    bool confirmEmail = false;
    bool password = false;
    bool confirmPassword = false;
};

struct account
{
    std::string email;
    std::string password;
};

int scene = 1, i = 1;
scn2 scene2;
scn3 scene3;
account acc[100];
bool first = true;

void input()
{
    std::ifstream fin("data.txt", std::ios::in | std::ios::out);
    while (fin >> acc[i].email)
    {
        fin >> acc[i].password;
        i++;
    }
    fin.close();
}

//-------------------------

bool trigger_event(Clock& clock, float& elapsedTime, float interval)
{
    Time pass = clock.getElapsedTime();
    elapsedTime += pass.asSeconds();
    clock.restart();
    if (elapsedTime >= interval)
    {
        elapsedTime = 0.0f;
        return true;
    }
    return false;
}

sf::Text createText(const std::string& fontPath, const std::string& textString, int characterSize, const sf::Color& fillColor, float posX, float posY)
{
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile(fontPath))
    {
        std::cerr << "Error loading font: " << fontPath << std::endl;
    }
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setFillColor(fillColor);
    text.setString(textString);
    text.setPosition(posX, posY);

    return text;
}

sf::RectangleShape createRectangleShape(float sizeX, float sizeY, float posX, float posY, const sf::Color& fillColor)
{
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(sizeX, sizeY));
    rectangle.setPosition(posX, posY);
    rectangle.setFillColor(fillColor);

    return rectangle;
}
void drawObjects(RenderWindow& window, Tetris& tetris, Sprite& backgroundSprite, RectangleShape& recBack,
    RectangleShape& recNext, RectangleShape& recScore, RectangleShape& recSpeed,
    RectangleShape& recNextShadow, RectangleShape& recScoreShadow, RectangleShape& recSpeedShadow,
    Text& nextBlock, Text& score, Text& speed)
{
    window.clear(Color(0, 0, 0));
    window.draw(backgroundSprite);
    window.draw(recBack);

    tetris.Draw(window);

    window.draw(recNextShadow);
    window.draw(recScoreShadow);
    window.draw(recSpeedShadow);
    window.draw(recNext);
    window.draw(recScore);
    window.draw(recSpeed);

    window.draw(nextBlock);
    window.draw(score);
    window.draw(speed);

    window.display();
}

void playGame(RenderWindow& window, ofstream &fout, int cell, int column, int row, int screen_size) {
    //text - ingame
    string font = "resoure/CallOfOpsDutyIi-7Bgm4.ttf";
    sf::Font f; int characterSize = 15; sf::Color fillColor = sf::Color::White;
    if (!f.loadFromFile(font))
    {
        std::cerr << "Error loading font: " << font << std::endl;
    }
    Text nextBlock;
    nextBlock.setFont(f);
    nextBlock.setCharacterSize(characterSize);
    nextBlock.setFillColor(fillColor);
    nextBlock.setString("Next");
    nextBlock.setPosition(95, 97);
    Text score;
    score.setFont(f);
    score.setCharacterSize(characterSize);
    score.setFillColor(fillColor);
    score.setString("Score");
    score.setPosition(92, 7);
    Text speed;
    speed.setFont(f);
    speed.setCharacterSize(characterSize);
    speed.setFillColor(fillColor);
    speed.setString("Speed");
    speed.setPosition(92, 51);

    //Game - inteerface
    Tetris tetris;

    RectangleShape cellShape(Vector2f(cell - 1, cell - 1));
    cellShape.setFillColor(Color(36, 36, 88));

    //background - ingame
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("resoure/new.jpg")) {
        std::cerr << "Error loading background texture" << std::endl;
        return;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(400) / backgroundTexture.getSize().x, static_cast<float>(170) / backgroundTexture.getSize().y);
    backgroundSprite.setPosition(-5, -5);

    //dropping
    sf::Clock clock;
    float elapsedTime = 0.0f;
    float interval = 0.5f; // 1 second interval
    window.clear(Color(0, 0, 0));

    //shapes
    sf::RectangleShape recScore = createRectangleShape(40, 20, 88, 26, sf::Color(39, 139, 75));
    sf::RectangleShape recScoreShadow = createRectangleShape(40, 20, 90, 28, sf::Color(11, 66, 35));
    sf::RectangleShape recSpeed = createRectangleShape(40, 20, 88, 70, sf::Color(173, 7, 31));
    sf::RectangleShape recSpeedShadow = createRectangleShape(40, 20, 90, 72, sf::Color(60, 0, 0));
    sf::RectangleShape recNext = createRectangleShape(40, 40, 88, 116, sf::Color(6, 171, 165));
    sf::RectangleShape recNextShadow = createRectangleShape(40, 40, 90, 118, sf::Color(0, 72, 78));
    sf::RectangleShape recBack = createRectangleShape(81, 161, 0, 0, sf::Color(30, 6, 64));
	window.setView(View(FloatRect(-3, -2, 140, 165)));
    while (window.isOpen())
    {
        if (first == true)
        {
            for (int j = 1; j < i; j++)
            {
                fout << acc[j].email << "\n";
                fout << acc[j].password << "\n";
            }
            first = false;
        }
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
            {
                tetris.hand_input(event);
            }
        }
        if (trigger_event(clock, elapsedTime, interval))
        {
            tetris.moveBlockDown();
        }
        {
            window.clear(Color(0, 0, 0));
            //cout << "clear" << endl;
        }
        window.draw(backgroundSprite);
        window.draw(recBack);

        {
            tetris.Draw(window);
            //cout << "draw" << endl;
        }
        //rec
        window.draw(recNextShadow);
        window.draw(recScoreShadow);
        window.draw(recSpeedShadow);
        window.draw(recNext);
        window.draw(recScore);
        window.draw(recSpeed);
        //text
        window.draw(nextBlock);
        window.draw(score);
        window.draw(speed);
        window.display();
    }
}

int main()
{
    const int cell = 8; // Adjust the cell size if needed
    const int column = 10; // Adjust the number of columns
    const int row = 20; // Adjust the number of rows
    const float screen_size = 1.0f;

    RenderWindow window(VideoMode(800, 900), "Tetris", Style::Close);

    //interface home / login
    Texture t1, t2, t3, t4, t5, t6, t7, t8, t9, t0;

    t0.loadFromFile("fontPic/startRec.png");
    t1.loadFromFile("fontPic/loginRec.png");
    t2.loadFromFile("fontPic/regisRec.png");
    t3.loadFromFile("fontPic/Email.png");
    t4.loadFromFile("fontPic/Password.png");
    t5.loadFromFile("fontPic/Submit.png");
    t6.loadFromFile("fontPic/Back.png");
    t7.loadFromFile("fontPic/ConfirmEmail.png");
    t8.loadFromFile("fontPic/ConfirmPassword.png");
    t9.loadFromFile("fontPic/LoginOk.png");

    Sprite start(t0);
    Sprite login(t1);
    Sprite registerr(t2);
    Sprite Email(t3);
    Sprite Password(t4);
    Sprite Submit(t5);
    Sprite Back(t6);
    Sprite ConfirmEmail(t7);
    Sprite ConfirmPassword(t8);
    Sprite LoginOK(t9);

    Font arial;
    arial.loadFromFile("fontPic/Arial.ttf");

    Textbox textLoginEmail(30, Color::White, false);
    textLoginEmail.setFont(arial);
    Textbox textLoginPassword(30, Color::White, false);
    textLoginPassword.setFont(arial);
    Textbox textRegisterEmail(30, Color::White, false);
    textRegisterEmail.setFont(arial);
    Textbox textRegisterConfirmEmail(30, Color::White, false);
    textRegisterConfirmEmail.setFont(arial);
    Textbox textRegisterPassword(30, Color::White, false);
    textRegisterPassword.setFont(arial);
    Textbox textRegisterConfirmPassword(30, Color::White, false);
    textRegisterConfirmPassword.setFont(arial);

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("fontPic/home.png")) {
        std::cerr << "Error loading background texture" << std::endl;
        return 1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(810) / backgroundTexture.getSize().x, static_cast<float>(900) / backgroundTexture.getSize().y);
    backgroundSprite.setPosition(-5, -5);

    input();

    std::ofstream fout("data.txt", std::ios::in | std::ios::out);

	while (window.isOpen())
	{
		if (first == true)
		{
			for (int j = 1; j < i; j++)
			{
				fout << acc[j].email << "\n";
				fout << acc[j].password << "\n";
			}
			first = false;
		}
		Event e;
		string new_mail, new_password;

		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				window.close();
			}
			if (e.type == Event::MouseButtonPressed)
			{
				if (e.key.code == Mouse::Left)
				{
					int x = Mouse::getPosition(window).x;
					int y = Mouse::getPosition(window).y;
					if (scene == 1)
					{
						if (x >= 170 && x <= 640 && y >= 470 && y <= 560)
						{
							playGame(window,fout,cell,column,row,screen_size);

						}
						if (x >= 233 && x <= 583 && y >= 607 && y <= 667)
						{
							// LOGIN
							scene = 2;
						}
						if (x >= 233 && x <= 583 && y >= 700 && y <= 760)
						{
							// REGISTER
							scene = 3;
						}
						if (x >= 720 && x <= 780 && y >= 17 && y <= 52)
						{
							// SETTING
						}
					}
					if (scene == 2)
					{
						// back
						if (x >= 233 && x <= 368 && y >= 530 && y <= 580)
						{
							scene = 1;
						}
						if (x >= 50 && x <= 200 && y >= 75 && y <= 125)
						{
							// click on email
							scene2.email = true;
							textLoginEmail.setSelected(true);
						}
						if (x >= 50 && x <= 259 && y >= 235 && y <= 285)
						{
							//click on password
							scene2.password = true;
							textLoginPassword.setSelected(true);
						}
						// submit
						if (x >= 143 && x <= 457 && y >= 405 && y <= 480)
						{
							std::string e, p;
							e = textLoginEmail.getText();
							p = textLoginPassword.getText();
							if (e.empty() == false && p.empty() == false)
							{
								for (int j = 1; j <= i; j++)
								{
									if ((e == acc[j].email && p == acc[j].password) ||
										(new_mail == acc[j].email && new_password == acc[j].password))
									{
										scene = 4;
									}
								}
							}
						}
					}
					if (scene == 3)
					{
						// back
						if (x >= 233 && x <= 368 && y >= 530 && y <= 580)
						{
							scene = 1;
						}
						// email
						if (x >= 15 && x <= 165 && y >= 34 && y <= 84)
						{
							scene3.email = true;
							textRegisterEmail.setSelected(true);
						}
						// confirm email
						if (x >= 15 && x <= 293 && y >= 116 && y <= 166)
						{
							scene3.confirmEmail = true;
							textRegisterConfirmEmail.setSelected(true);
						}
						// password
						if (x >= 15 && x <= 224 && y >= 198 && y <= 248)
						{
							scene3.password = true;
							textRegisterPassword.setSelected(true);
						}
						// confirm password
						if (x >= 15 && x <= 329 && y >= 280 && y <= 330)
						{
							scene3.confirmPassword = true;
							textRegisterConfirmPassword.setSelected(true);
						}
						//submit
						if (x >= 143 && x <= 457 && y >= 405 && y <= 480)
						{
							std::string e, ce, p, cp;
							e = textRegisterEmail.getText();
							ce = textRegisterConfirmEmail.getText();
							p = textRegisterPassword.getText();
							cp = textRegisterConfirmPassword.getText();
							if (e.empty() == false && ce.empty() == false && p.empty() == false && cp.empty() == false)
							{
								if (e == ce && p == cp)
								{
									new_mail = e;
									new_password = p;
									fout << e << "\n";
									fout << p << "\n";
								}
							}
						}
					}
					if (scene == 4) {
						if (x >= 300 && x <= 500 && y >= 400 && y <= 500) {
							playGame(window,fout,cell,column,row, screen_size);
						}
					}
				}
			}
			if (e.type == Event::TextEntered)
			{
				if (scene == 2)
				{
					if (scene2.email == true)
					{
						if (Keyboard::isKeyPressed(Keyboard::Return))
						{
							textLoginEmail.setSelected(false);
							scene2.email = false;
						}
						else
						{
							textLoginEmail.typedOn(e);
						}
					}
					else if (scene2.password == true)
					{
						if (Keyboard::isKeyPressed(Keyboard::Return))
						{
							textLoginPassword.setSelected(false);
							scene2.password = false;
						}
						else
						{
							textLoginPassword.typedOn(e);
						}
					}
				}
				if (scene == 3)
				{
					if (scene3.email == true)
					{
						if (Keyboard::isKeyPressed(Keyboard::Return))
						{
							textRegisterEmail.setSelected(false);
							scene3.email = false;
						}
						else
						{
							textRegisterEmail.typedOn(e);
						}
					}
					else if (scene3.confirmEmail == true)
					{
						if (Keyboard::isKeyPressed(Keyboard::Return))
						{
							textRegisterConfirmEmail.setSelected(false);
							scene3.confirmEmail = false;
						}
						else
						{
							textRegisterConfirmEmail.typedOn(e);
						}
					}
					else if (scene3.password == true)
					{
						if (Keyboard::isKeyPressed(Keyboard::Return))
						{
							textRegisterPassword.setSelected(false);
							scene3.password = false;
						}
						else
						{
							textRegisterPassword.typedOn(e);
						}
					}
					else if (scene3.confirmPassword == true)
					{
						if (Keyboard::isKeyPressed(Keyboard::Return))
						{
							textRegisterConfirmPassword.setSelected(false);
							scene3.confirmPassword = false;
						}
						else
						{
							textRegisterConfirmPassword.typedOn(e);
						}
					}
				}
			}
		}
		window.draw(backgroundSprite);
		if (scene == 1)
		{
			login.setPosition(233, 607);
			login.setScale(static_cast<float>(810) / backgroundTexture.getSize().x, static_cast<float>(600) / backgroundTexture.getSize().y);
			registerr.setPosition(233, 720);
			registerr.setScale(static_cast<float>(810) / backgroundTexture.getSize().x, static_cast<float>(750) / backgroundTexture.getSize().y);
			start.setPosition(170, 470);
			start.setScale(static_cast<float>(810) / backgroundTexture.getSize().x, static_cast<float>(900) / backgroundTexture.getSize().y);

			Font f; f.loadFromFile("fontPic/game.ttf");
			Text gameName;
			gameName.setFont(f);
			gameName.setCharacterSize(80);
			gameName.setFillColor(Color(226, 226, 227));
			gameName.setString("TETRIS");
			gameName.setPosition(280, 130);

			Text grName;
			grName.setFont(f);
			grName.setCharacterSize(50);
			grName.setFillColor(Color(226, 226, 227));
			grName.setString("Gr Tennn");
			grName.setPosition(310, 215);

			Text startText;
			startText.setFont(f);
			startText.setCharacterSize(80);
			startText.setFillColor(Color(249, 214, 7));
			startText.setOutlineColor(Color::Black);
			startText.setOutlineThickness(3);
			startText.setString("START");
			startText.setPosition(295, 455);

			Text loginText;
			loginText.setFont(f);
			loginText.setCharacterSize(50);
			loginText.setFillColor(Color(249, 214, 7));
			loginText.setOutlineColor(Color::Black);
			loginText.setOutlineThickness(3);
			loginText.setString("Login");
			loginText.setPosition(345, 602);

			Text regisText;
			regisText.setFont(f);
			regisText.setCharacterSize(50);
			regisText.setFillColor(Color(249, 214, 7));
			regisText.setOutlineColor(Color::Black);
			regisText.setOutlineThickness(3);
			regisText.setString("Register");
			regisText.setPosition(315, 695);

			window.draw(gameName);
			window.draw(grName);
			window.draw(start);
			window.draw(login);
			window.draw(registerr);
			window.draw(startText);
			window.draw(loginText);
			window.draw(regisText);
		}
		if (scene == 2)
		{
			sf::Texture loginBackgroundTexture;
			if (!loginBackgroundTexture.loadFromFile("fontPic/loginBack.jpg")) {
				std::cerr << "Error loading background texture" << std::endl;
				return 1;
			}
			sf::Sprite loginBack(loginBackgroundTexture);
			loginBack.setScale(static_cast<float>(810) / loginBackgroundTexture.getSize().x, static_cast<float>(900) / loginBackgroundTexture.getSize().y);
			loginBack.setPosition(-5, -5);

			window.draw(loginBack);
			Submit.setPosition(143, 405);
			window.draw(Submit);
			Password.setPosition(50, 235);
			window.draw(Password);
			Email.setPosition(50, 75);
			window.draw(Email);
			Back.setPosition(233, 530);
			window.draw(Back);
			textLoginEmail.setPosition({ 210,76 });
			textLoginEmail.drawTo(window);
			textLoginPassword.setPosition({ 269,237 });
			textLoginPassword.drawTo(window);
			//window.draw(te);
		}
		if (scene == 3)
		{
			sf::Texture loginBackgroundTexture;
			if (!loginBackgroundTexture.loadFromFile("fontPic/loginBack.jpg")) {
				std::cerr << "Error loading background texture" << std::endl;
				return 1;
			}
			sf::Sprite regBack(loginBackgroundTexture);
			regBack.setScale(static_cast<float>(810) / loginBackgroundTexture.getSize().x, static_cast<float>(900) / loginBackgroundTexture.getSize().y);
			regBack.setPosition(-5, -5);
			window.draw(regBack);
			Submit.setPosition(143, 405);
			window.draw(Submit);
			Back.setPosition(233, 530);
			window.draw(Back);
			Email.setPosition(15, 34);
			window.draw(Email);
			ConfirmEmail.setPosition(15, 116);
			window.draw(ConfirmEmail);
			Password.setPosition(15, 198);
			window.draw(Password);
			ConfirmPassword.setPosition(15, 280);
			window.draw(ConfirmPassword);
			textRegisterEmail.setPosition({ 175, 35 });
			textRegisterEmail.drawTo(window);
			textRegisterConfirmEmail.setPosition({ 303,117 });
			textRegisterConfirmEmail.drawTo(window);
			textRegisterPassword.setPosition({ 234,199 });
			textRegisterPassword.drawTo(window);
			textRegisterConfirmPassword.setPosition({ 339,281 });
			textRegisterConfirmPassword.drawTo(window);
		}
		if (scene == 4)
		{
			sf::Texture loginBackgroundTexture;
			if (!loginBackgroundTexture.loadFromFile("fontPic/loginBack.jpg")) {
				std::cerr << "Error loading background texture" << std::endl;
				return 1;
			}
			sf::Sprite loginBack(loginBackgroundTexture);
			loginBack.setScale(static_cast<float>(810) / loginBackgroundTexture.getSize().x, static_cast<float>(900) / loginBackgroundTexture.getSize().y);
			loginBack.setPosition(-5, -5);

			RectangleShape playButton = createRectangleShape(200, 100, 300, 400, Color:: Yellow);
			
			window.draw(loginBack);
			window.draw(playButton);
			LoginOK.setPosition(300, 219);
			window.draw(LoginOK);
		}
		window.display();
	}
    window.close();
    return 0;
}
