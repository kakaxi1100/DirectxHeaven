#include <stdio.h>
#include <string.h>
#include <error.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

static int l_sin(lua_State *L)
{
	double d = luaL_checknumber(L, 1);
	lua_pushnumber(L, sin(d));
	return 1;
}

int main(void)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	lua_pushcfunction(L, l_sin);
	lua_setglobal(L, "mysin");
	if(luaL_loadfile(L, "1.lua") || lua_pcall(L,0,0,0)){
		printf("cannnot run config. file :%s", lua_tostring(L, -1));
	}
		
//	lua_close(L);
	system("pause");
	return 0;
}

/*int main(void)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	if(luaL_loadfile(L, "1.lua") || lua_pcall(L,0,0,0)){
		printf("cannnot run config. file :%s", lua_tostring(L, -1));
	}
	
	lua_getglobal(L, "f");
	lua_pushnumber(L, 10);
	lua_pushnumber(L, 8);
	
	if(lua_pcall(L, 2, 2, 0) != LUA_OK){
		printf("Error!\n");
	}
	
	float a = lua_tonumber(L, -1);
	float b = lua_tonumber(L, -2);
	printf("%g, %g\n", a,b);
	
	system("pause");
	return 0;
}*/

/*int main(void)
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	
	if(luaL_loadfile(L, "1.lua") || lua_pcall(L,0,0,0)){
		printf("cannnot run config. file :%s", lua_tostring(L, -1));
	}
	lua_getglobal(L, "background");
	lua_pushstring(L, "r");
	lua_gettable(L, -2);
	lua_pushstring(L,"b");
	lua_gettable(L, -3);
	lua_pushstring(L, "g");
	lua_gettable(L, -4);
	
	float red = lua_tonumber(L, -3);
	float blue = lua_tonumber(L, -2);
	float green = lua_tonumber(L, -1);
	
	printf("%g, %g, %g \n", red, green, blue);
	
	lua_pop(L, 1);
	int n = lua_gettop(L);
	
	printf("%d \n", n);
	
	system("pause");
	return 0;
}*/

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
