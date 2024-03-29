namespace mes{

#define _S const std::string
#define _V const std::vector<std::string>

#ifdef __MES_UI__
	_S welcome = "Welcome to the vocabulary training app thingy (that currently posesses no proper name).\nEnter 'h' for help menu or 'l' to change language\n";

	_S wait = "Press ENTER to continue...";

	//Help messages
	_S help = std::string("Currently supported commands:\n")+
			"h\t	display this menu\n"+
			"l\t	change language (currently unsupported)\n"+
			"q\t	exit program\n"+
			"c\t	clear screen\n"+
			"i\t	import new set from source file\n"+
			"t\t	train current set\n";

	_S lang = "English is the only supported language for now.\n";

	//Wrong command
	_V err_com = {
		"The command '",
		"' does not exist. Enter 'h' for list of available commands.\n"};
	_V err_arg = {
		"Invalid argument(s) ",
		"for command ",
		"Command ",
		" needs argument(s)",
		". Type h <command> for help with command.\n"};


	_S exit="Exiting...\n";

	//Quiz functionality
	_V import = {
		"Enter path to training file:\n",
		"File read successfully.\n",
		"File not found: "};
	_S err_fnf = "File not found: ";

	_V start_sf = {
		"Training sets from source file:\n",
		"\nList of sets in file:\n"};
	_S fin = "Finished all sets\n";
#endif

#ifdef __MES_TRAIN__

	_V no_ent = {
		"No active entries in set: ",
		" . Skipping.\n"};

	_V err_line = {
		"Line ",
		" is not formated correctly:\n"};
	_V q_train = {
		"Train set ",
		"? y/N\n"};
	_S cor = "Correct\n";
	_S inc = "Incorrect\n";
	_S ovr = "Enter override or press ENTER to continue: ";
	_S ovrd = "OVERRIDEN: ";

	_S set_end = "Finished set. Score:\n";

#endif

#undef _S
#undef _V

}
