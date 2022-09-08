/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smun <smun@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 16:23:53 by smun              #+#    #+#             */
/*   Updated: 2022/09/08 20:08:37 by smun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "std.hpp"

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}
	SDL_Window* win = SDL_CreateWindow(
		"scop",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN);

	bool running = true;
	for(;running;)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					running = false;
					break;
			}
		}
	}

	SDL_DestroyWindow(win);
	SDL_Quit();

	return EXIT_SUCCESS;
}
