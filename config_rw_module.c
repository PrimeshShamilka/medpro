#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include <string.h>

struct user {
  char *username, *password, *user_type;
};

struct privileges {
  char *r_privilege, *w_privilege;
};


/*
*
* GET USER 
* 
*/

struct user get_user_cfg(char* username_input)
{
  config_t cfg;
  config_setting_t *setting;
  const char *str;

  config_init(&cfg);

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, "/home/primesh/medpro/medpro.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    // return(EXIT_FAILURE);
  }

  setting = config_lookup(&cfg, "users");
  if(setting != NULL)
  {
    int count = config_setting_length(setting);
    int i;
    for(i = 0; i < count; ++i)
    {
      config_setting_t *user = config_setting_get_elem(setting, i);

      /* Only output the record if all of the expected fields are present. */
      const char *username, *password, *user_type;

      if(!(config_setting_lookup_string(user, "username", &username)
        && config_setting_lookup_string(user, "password", &password)
        && config_setting_lookup_string(user, "user_type", &user_type)))
      continue;

      if(strcmp(username, username_input) == 0){
        struct user result = {username, password, user_type};
        return result;
      }
    } 
  }

}



/*
*
* GET USER LIST
* 
*/

int get_user_list_cfg()
{
  config_t cfg;
  config_setting_t *setting;
  const char *str;

  config_init(&cfg);

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, "/home/primesh/medpro/medpro.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  /* Output a list of all users. */
  setting = config_lookup(&cfg, "users");
  if(setting != NULL)
  {
    int count = config_setting_length(setting);
    int i;
    for(i = 0; i < count; ++i)
    {
      config_setting_t *user = config_setting_get_elem(setting, i);

      /* Only output the record if all of the expected fields are present. */
      const char *username, *password, *user_type;

      if(!(config_setting_lookup_string(user, "username", &username)
           && config_setting_lookup_string(user, "password", &password)
           && config_setting_lookup_string(user, "user_type", &user_type)))
        continue;
      printf("%s  %s  %s\n", username, password, user_type);
    }
    putchar('\n');
  }

}



/*
*
* ADD USER
* 
*/


int add_user_cfg(char *username, char *hashed_password, char *user_type){

  static const char *output_file = "/home/primesh/medpro/medpro.cfg";
  config_t cfg;
  config_setting_t *root, *setting, *user;

  config_init(&cfg);
  config_set_options(&cfg,
                     (CONFIG_OPTION_FSYNC
                      | CONFIG_OPTION_SEMICOLON_SEPARATORS
                      | CONFIG_OPTION_COLON_ASSIGNMENT_FOR_GROUPS
                      | CONFIG_OPTION_OPEN_BRACE_ON_SEPARATE_LINE));

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, "/home/primesh/medpro/medpro.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  /* Find the 'users' setting. Add intermediate settings if they don't yet
  * exist.
  */
  root = config_root_setting(&cfg);

  setting = config_setting_get_member(root, "users");
  if(!setting)
    setting = config_setting_add(setting, "users", CONFIG_TYPE_LIST);

  // /* Create the new user entry. */
  user = config_setting_add(setting, NULL, CONFIG_TYPE_GROUP);

  setting = config_setting_add(user, "username", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, username);

  setting = config_setting_add(user, "password", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, hashed_password);

  setting = config_setting_add(user, "user_type", CONFIG_TYPE_STRING);
  config_setting_set_string(setting, user_type);


  /* Write out the updated configuration. */
  if(! config_write_file(&cfg, output_file))
  {
    fprintf(stderr, "Error while writing file.\n");
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  fprintf(stderr, "USER ADDED SUCCESSFULLY!.\n",
          output_file);

  config_destroy(&cfg);
  return(EXIT_SUCCESS);
}



/*
*
* GET USER PRIVILEGE
* 
*/

struct privileges get_user_privileges(char *db_location){
  config_t cfg;
  config_setting_t *setting;
  const char *str;

  config_init(&cfg);

  /* Read the file. If there is an error, report it and exit. */
  if(! config_read_file(&cfg, "/home/primesh/medpro/medpro.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    // return(EXIT_FAILURE);
  }

  setting = config_lookup(&cfg, db_location);
  if(setting != NULL)
  {
    int count = config_setting_length(setting);
    char *read_privilege = config_setting_get_string_elem(setting, 0);
    char *write_privilege = config_setting_get_string_elem(setting, 1);
    // printf("%s %s", read_privilege, write_privilege);

    struct privileges usr_privilege = {read_privilege, write_privilege};
    return usr_privilege;
  }
}


