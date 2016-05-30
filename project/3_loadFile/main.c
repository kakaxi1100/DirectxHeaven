#include <stdio.h>
#include <string.h>
#include <error.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main(void)
{
	
	system("pause");
	return 0;
}

/*void load(lua_State *L, const char *fname, int *w, int *h){
	if(luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0)){
		printf("cannnot run config. file :%s", lua_tostring(L, -1));
	}
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if(!lua_isnumber(L, -2)){
		printf("'width' should be a number \n");
	}
	if(!lua_isnumber(L, -1)){
		printf("'height' should be a number \n");
	}
	
	*w = lua_tointeger(L, -2);
	*h = lua_tointeger(L, -1);
	printf("width: %d, height: %d\n",*w, *h);
}

int main(void)
{
	char buff[256];
	int error;
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	int w;
	int h;
	load(L, "1.lua", &w, &h);
	
	lua_close(L);
	system("pause");
	return 0;
}*/
