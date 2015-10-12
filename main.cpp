//@author Atte Pohjanmaa 2013-2014

#include <cerrno>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "/usr/include/SDL/SDL.h"
#include <string>
#include <fstream>
int main()
{

	int exit = 0;

	std::string teksti = "";
	std::string merkki = "";

	char lopullinen[256];

	SDL_Event tapahtuma;
	SDL_Event tapahtuma2;
	SDL_Event tapahtuma3;

	bool valmis = false;
	bool tvalmis = false;
	/*Perus initit*/
	if( SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
            fprintf( stderr, "Could not initialise SDL: %s\n", SDL_GetError() );
            return 1;
        }

       /*luodaan ikkunaa*/
        if( !SDL_SetVideoMode( 320, 200, 0, 0 ) ){
            fprintf( stderr, "Could not set video mode: %s\n", SDL_GetError() );
            SDL_Quit();
            return 1;
        }

        SDL_EnableUNICODE( 1 );
	while(exit == 0)
	{
	A:
	//1. poll event
		while(SDL_PollEvent(&tapahtuma))
		{

			if(tapahtuma.type == SDL_KEYDOWN)
			{
			    //Kun Q on painettu, niin lopeta
				if(tapahtuma.key.keysym.sym == SDLK_q)
				{
					SDL_Quit();
					return 0;
				}
				//vasen ctrl painettu
				else if(tapahtuma.key.keysym.sym == SDLK_LCTRL)
				{
					tvalmis = false;
					//Looppi, jolla odotetaan toista nappulaa
					while(tvalmis == false)
					{
						while(SDL_PollEvent(&tapahtuma2))
						{
							//Looppi, jolla odotetaan napin painallusta.
							if(tapahtuma2.type == SDL_KEYDOWN)
							{
								char tiedosto2nimi[256];
								
								std::string merkki2 = SDL_GetKeyName(tapahtuma2.key.keysym.sym);

								//hae tiedostosta
								//Luetaan näppäimen niminen tiedosto ja puhutaan se.
								sprintf(tiedosto2nimi, "%s.txt", merkki2.c_str());

								std::ifstream tiedosto;
								tiedosto.open(tiedosto2nimi);

								teksti = "";
								printf("luetaan filua: %s \n", tiedosto2nimi);
								if(tiedosto.is_open())
								{
									while( getline(tiedosto, teksti))
									{

										char textisi[256];
										sprintf(textisi, "espeak -v fi \"%s\"", teksti.c_str());
										system(textisi);
										printf("teksti: %s\n", teksti.c_str());
									}
									tiedosto.close();
								}
								else
								{
									printf("filun luku ei onnistu \n");
									//siirrä tvalmis yhtä riviä alemmas, jos haluat errorcoden
									tvalmis = true;
									std::cerr << "Error: " << strerror(errno) << "\n";
									
								}
								tvalmis = true;
								//Tvalmiin ollessa true looppi loppuu

							}

						}
					}

				}
				//Kun oikea ctrl painetaan, tallennetaan teksti tiedostoon
				else if(tapahtuma.key.keysym.sym == SDLK_RCTRL)
				{
					//loopin ehtomuuttuja
					valmis = false;

					//toista
					printf("oikea kontrol painettu \n");
					while(valmis == false)
					{
						while(SDL_PollEvent(&tapahtuma3))
						{

							if(tapahtuma3.type == SDL_KEYDOWN)
							{
								char tiedosto3nimi[256];

								std::string merkki3 = SDL_GetKeyName(tapahtuma3.key.keysym.sym);

								//Tallenna tiedostoon ja tee ääni, joka kertoo, että teksti on tallennettu.

								sprintf(tiedosto3nimi, "%s.txt", merkki3.c_str());

								std::ofstream tiedosto (tiedosto3nimi);
								if(tiedosto.is_open())
								{
									tiedosto << teksti;
									tiedosto.close();
								}
								else
								{
									printf("filun kirjoitus ei onnistu");
								}
								valmis = true;

							}

						}
					}


				}
				//enter
				else if(tapahtuma.key.keysym.sym == SDLK_RETURN)
				{

					sprintf(lopullinen, "espeak -v fi \"%s\"",teksti.c_str());
					system(lopullinen);

				}
				//Tekstin poisto merkki kerrallaan
				else if(tapahtuma.key.keysym.sym == SDLK_BACKSPACE)
				{
					printf("poistetaan merkki \n");
					
					
					
					//teksti = (teksti.length()-1);
					
					

					if(teksti != "")
					{
						teksti.erase(teksti.begin() + teksti.size() - 1);
					}
					printf("teksti: %s \n", teksti.c_str());
				}
				else
				{
					//TODO laita rpi kirjoittaamaan ruudulle merkit

					merkki = "";
					merkki = (SDL_GetKeyName(tapahtuma.key.keysym.sym));
					if(merkki.length() == 1 || merkki.length() == 0)
					{
						teksti.append(merkki);
					}
					else if(tapahtuma.key.keysym.sym == SDLK_SPACE)
					{
						teksti.append(" ");
					}
					else if((int)tapahtuma.key.keysym.sym == 228)
					{
						teksti.append("ä");
					}
					else if((int)tapahtuma.key.keysym.sym == 246)
					{
						teksti.append("ö");
					}
					else if((int)tapahtuma.key.keysym.sym == 229)
					{
						teksti.append("å");
					}
				}
					//Näytölle lähetys

					std::ofstream merkkijonofilu ("merkkijono.txt");
					if(merkkijonofilu.is_open())
					{
						merkkijonofilu << teksti;
						merkkijonofilu.close();
						system("python tekstikuvaksi.py");
					}
					else
					{
						printf("koko merkkijonon kirjoitus tiedostoon merkkijono.txt epäonnistui");
					}
					printf("%s \n", teksti.c_str());
				
			}

		}

	}

}
