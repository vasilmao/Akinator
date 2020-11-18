DEF_CMD(search, "Search in database", 1, {
    AKsearch(&tree);
})
DEF_CMD(get_characteristics, "Get characteristics of entity in database", 2, {
    AKgetCharacteristics(&tree);
})
DEF_CMD(save_to_file, "Save database to file", 3, {
    AKsave(&tree);
})
DEF_CMD(exit, "Exit program", 10, {
    printf("Goodbye!\n");
    AKdestroy(&tree);
    return 0;
})
