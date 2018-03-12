#ifndef	TERMINAL_HPP
# define TERMINAL_HPP
# include <unistd.h>
# include <termcap.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <curses.h>
# include <string>

static int		mynputc(int c)
{
	write(0, &c, sizeof(c));
	return (c);
}

namespace		fx
{
	class		Terminal
	{
		struct	state
		{
			int							_cur;
			std::string					_disp;
		};

		bool							_status;
		int								_fd;
		char*							_type;
		int								_o_x;
		int								_o_y;
		int								_cur;
		int								_scr_x;
		int								_scr_y;
		struct termios					_term;
		struct termios					_backup;
		char							_buffer[1024];
		char							_rbuffer[3];
		std::string						_disp;
		std::vector<Terminal::state>	_states;

		void			getScreenSize(void)
		{
			if (this->_status)
			{
				this->_scr_x = tgetnum("co");
				this->_scr_y = tgetnum("li");
			}			
		}

		void			calcSize(void)
		{
			this->getScreenSize();
			this->_o_x = this->_disp.length() % this->_scr_x;
			this->_o_y = this->_disp.length() / this->_scr_y;
		}

		void			debug(char *mbuf)
		{
			FILE		*f = fopen("DEBUG", "a");
			char		buf[1024] = {0};
			if (f)
			{
				sprintf(buf, "cur = %d, buf = %s\n", this->_cur, mbuf);
				fwrite(buf, strlen(buf), 1, f);
				fclose(f);
			}
		}

		void			insert(char *buf)
		{
			this->debug(buf);
			this->_disp = this->_disp.insert(this->_cur, std::string(buf));
			this->_cur++;
			tputs(tgetstr("im", NULL), 1, mynputc);
			tputs(tgetstr("ic", NULL), 1, mynputc);
			write(STDOUT_FILENO, buf, 1);
			tputs(tgetstr("ip", NULL), 1, mynputc);
			tputs(tgetstr("ei", NULL), 1, mynputc);
		}

		void			remove(void)
		{
			char		buf[3] = {27, 91, 68};

			if (0 < this->_cur && this->_cur < (int)this->_disp.length())
			{
				this->_disp = this->_disp.erase(this->_cur, 1);
				this->_cur--;
				tputs(tgetstr("dm", NULL), 1, mynputc);
				tputs(tgetstr("dc", NULL), 1, mynputc);
				tputs(tgetstr("ed", NULL), 1, mynputc);
				this->treatInput((char*)buf);
			}
		}

		public :

			Terminal(const int fd = STDIN_FILENO):
				_status(true),
				_fd((int)fd),
				_type(getenv("TERM")),
				_o_x(0),
				_o_y(0),
				_cur(0),
				_scr_x(0),
				_scr_y(0),
				_disp(""),
				_states(std::vector<Terminal::state>())
			{
				int			ok = 0;

				if (!this->_type)
					this->_status = false;
				else if ((ok = tgetent(this->_buffer, this->_type)) < 0)
					this->_status = false;
				else if (tcgetattr(this->_fd, &this->_term) == -1)
					this->_status = false;
				else
					tcgetattr(this->_fd, &this->_backup);
			}

			int		canonMode(int umode = TCSADRAIN)
			{
				if (this->_status)
				{
					this->_term.c_lflag &= ~(ICANON);
					this->_term.c_lflag &= ~(ECHO);
					this->_term.c_cc[VMIN] = 1;
					this->_term.c_cc[VTIME] = 0;
					//this->resetCursorPosition();
				}
				return (this->update(umode));
			}

			int		textMode(int umode = TCSADRAIN)
			{
				if (this->_status)
				{
					this->_term.c_lflag &= ~(ICANON);
					this->_term.c_lflag &= ~(ECHO);
					this->_term.c_cc[VMIN] = 1;
					this->_term.c_cc[VTIME] = 0;
					//this->resetCursorPosition();
					tputs(tgetstr("am", NULL), 1, mynputc);
				}
				return (this->update(umode));
			}

			std::string		getInputBuffer(void)
			{
				return (this->_disp);
			}

			struct termios&	getTermios()
			{
				return (this->_term);
			}

			void	save_state(void)
			{
				Terminal::state		s;

				s._cur = this->_cur;
				s._disp = this->_disp;
				this->_states.push_back(s);
			}

			int		update(int umode = TCSADRAIN)
			{
				if (this->_status)
					return (tcsetattr(this->_fd, umode, &this->_term));
				return (-1);
			}

			int		reset(int umode = TCSADRAIN)
			{
				if (this->_status)
					return (tcsetattr(this->_fd, umode, &this->_backup));
				return (-1);
			}

			void	resetInput(void)
			{
				this->_disp = "";
				this->_cur = 0;
			}

			char*	getInput(void)
			{
				this->_rbuffer[0] = 0;
				this->_rbuffer[1] = 0;
				this->_rbuffer[2] = 0;
				read(this->_fd, this->_rbuffer, 3);
				return (this->_rbuffer);
			}

			int		treatInput(char* buf, int umode = TCSADRAIN)
			{
				//printf("%d %d %d\n", buf[0], buf[1], buf[2]);
				this->calcSize();
				//Up
				if (buf[0] == 27 && buf[1] == 91 && buf[2] == 65)
				{
					if (this->_cur <= 0)
						this->_cur = 0;
					else if (this->_cur - this->_scr_x > 0)
					{
						tputs(tgetstr("up", NULL), 1, mynputc);
						this->_cur -= this->_scr_x;
					}
					else
					{
//						tputs(tgetstr("up", NULL), 1, mynputc);
						while (this->_cur > 0)
						{
							tputs(tgetstr("le", NULL), 1, mynputc);
							this->_cur--;
						}
					}
				}
				//Down
				else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 66)
				{
					ssize_t	max = this->_disp.length();
					if (this->_cur >= max)
						this->_cur = max;
					else if (this->_cur + this->_scr_x < max)
					{
						tputs(tgetstr("do", NULL), 1, mynputc);
						this->_cur += this->_scr_x;
					}
					else
					{
						while (this->_cur < max)
						{
							tputs(tgetstr("nd", NULL), 1, mynputc);
							this->_cur++;
						}
					}
				}
				//Left
				else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 68)
				{
					if (this->_cur > 0)
					{
						tputs(tgetstr("le", NULL), 1, mynputc);
						this->_cur--;
					}
					else
						this->_cur = 0;
				}
				//Right
				else if (buf[0] == 27 && buf[1] == 91 && buf[2] == 67)
				{
					ssize_t	max = this->_disp.length();
					if (this->_cur >= max)
						this->_cur = max;
					else
					{
						tputs(tgetstr("nd", NULL), 1, mynputc);
						this->_cur++;
					}
				}
				else if ((int)buf[0] == 127)
					this->remove();
				else
				{
					this->insert(buf);
					return (0);
				}
				return (1);
				(void)umode;
			}
	};
}

#endif