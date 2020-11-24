DEF_CMD(search, "Search in database", 1, {
    AKsearch(&tree);
})
DEF_CMD(print_characteristics, "Get characteristics of entity in database", 2, {
    AKsay("What characteristics you want to see?\n");
    char* entity_name = scanString();
    AKprintCharacteristics(&tree, entity_name);
    destroyString(entity_name);
})
DEF_CMD(compare_characteristics, "Comapre characteristics of two entities", 3, {
    AKsay("What characteristics you want to compare?\nFirst  is: ");
    char* entity_name1 = scanString();
    AKsay("Second is: ");
    char* entity_name2 = scanString();
    AKcompareCharateristics(&tree, entity_name1, entity_name2);
    destroyString(entity_name1);
    destroyString(entity_name2);
})
DEF_CMD(save_to_file, "Save database to file", 4, {
    AKsay("Enter file name: ");
    char* filename = scanString();
    AKsaveToFile(&tree, filename);
})
DEF_CMD(load_from_file, "Load database from file", 5, {
    AKsay("Enter file name: ");
    char* filename = scanString();
    AKloadFromFile(&tree, filename);
})
DEF_CMD(exit, "Exit program", 10, {
    AKsay("Goodbye!\n");
    AKdestroy(&tree);
    return 0;
})
