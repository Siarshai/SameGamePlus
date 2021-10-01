#ifndef SAME_GAME_PLUS_SCOREBOARD_FACTORY_H
#define SAME_GAME_PLUS_SCOREBOARD_FACTORY_H

#include <memory>
#include "iscoreboard.h"


/*
 * There is no really much sense to decompose scoreboards like that right now :)
 * And this is not really a factory either.
 * Seems like overengineering, but in real projects you do need stop-gag gates like this.
 * But let's pretend it's actually big module with lots of private dependencies, okay?
 *
 * shared_ptr is used because IScoresToBackend is referenced both from main GameBackend class
 * (adding new scores) and scores tables model (displaying scores) - none have unique
 * control over it
 */
std::shared_ptr<IScoreBoard> get_scoreboard(const QString& scoreboard_filepath, int scores_num);


#endif //SAME_GAME_PLUS_SCOREBOARD_FACTORY_H
