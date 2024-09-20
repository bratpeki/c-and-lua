#include <stdio.h> /* Printing */
#include <stdlib.h> /* Exit codes */

#include <lua.h> /* The Lua core */
#include <lualib.h> /* The standard Lua library */
#include <lauxlib.h> /* Practically, the C API */
#include <luaconf.h> /* Contains Lua datatypes */

int main() {

	lua_State *L; /* Lua state */
	LUA_NUMBER result; /* Addition result */

	L = luaL_newstate(); /* Create a new Lua state */
	luaL_openlibs(L); /* Load the Lua standard libraries */

	/* Load and execute 'functions.lua' */
	if (luaL_dofile(L, "functions.lua") != LUA_OK) {
		fprintf(
			stderr,
			"There was an error loading 'functions.lua' (%s)! Closing.\n",
			lua_tostring(L, -1)
		);
		lua_close(L);
		return EXIT_FAILURE;
	}

	/* printHello() */

	lua_getglobal(L, "printHello"); /* Push the 'printHello' function onto the stack */

	/*
	 * The first 0 is the number of args
	 * The second 0 is the number of return values
	 * The third 0 is something called "errfunc", no idea what it does
	 */
	if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
		fprintf(
			stderr,
			"There was an error running 'printHello' (%s)! Closing.\n",
			lua_tostring(L, -1) /* The error message is at the top of the stack */
		);
		lua_close(L);
		return 1;
	}

	/* add(5, 7) */

	lua_getglobal(L, "add"); /* Push the 'add' function onto the stack */
	lua_pushnumber(L, 5); /* Pushing the two arguments */
	lua_pushnumber(L, 7);

	if (lua_pcall(L, 2, 1, 0) != LUA_OK) { /* Call the function (2 args, 1 return value) */
		fprintf(
			stderr,
			"There was an error running 'add' (%s)! Closing.\n",
			lua_tostring(L, -1) /* The error message is at the top of the stack */
		);
		lua_close(L);
		return 1;
	}

	/*
	 * Getting the result from the top of the stack and displaying it.
	 * The if statement is there just to show you how it's done;
	 * since Lua is dynamically typed, so we might not get a number back!
	 */
	if (lua_isnumber(L, -1)) {
		result = lua_tonumber(L, -1);
		printf("Result of add(5, 7): %.0f\n", result);
	}

	/*
	 * This pops one thing off the stack.
	 * In our case, that's the result of 'add'.
	 * It's not necessary, since we're calling lua_close immediately after it,
	 * but I'm leaving it here to, again, show you how to pop any number of elements off.
	 */
	lua_pop(L, 1);

	lua_close(L); /* Clean up and close the Lua state */

	return EXIT_SUCCESS;

}
