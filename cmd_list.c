#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "cmd.h"
#include "cmd_list.h"


cdb_node_t cmd_root[] = {
	{
		CMD_MODE_ROOT,
        CMD_TYPE_CMD,
		"enable",
		"Enable Command prompt for configuration",
		enable_cmd_prompt,
		NULL,
		CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
	}
};

cdb_node_t cmd_cfg_terminal[] = {
	{
		CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
		"terminal",
		"Config Terminal",
		enable_config_terminal,
		NULL,
		CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
	}
};

cdb_node_t cfg_if_type[] = {
	{
		CMD_MODE_IF,
        CMD_TYPE_CMD,
		"ethernet",
		"Ethernet Interface",
		config_if_type_enet,
		cfg_if_list,
		CMD_FLAG_NEXT
	},
    {
		CMD_MODE_IF,
        CMD_TYPE_CMD,
		"mgmt",
		"Management Interface",
		config_if_type_mgmt,
		cfg_if_list,
		CMD_FLAG_LAST | CMD_FLAG_NEXT
	},
};

cdb_node_t cfg_if_list[] = {
	{
		CMD_MODE_NONE,
        CMD_TYPE_IF_STRING,
		"",
		"If Or If List (1,3-5,7) ",
		config_if_list,
		NULL,
		CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
	}
};

cdb_node_t cfg_set_str [] = {
    {
        CMD_MODE_NONE,
        CMD_TYPE_STRING,
        "",
        "Set Cmd String",
        config_set_string,
        NULL,
        CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
    },
};

cdb_node_t cmd_cfg_if[] = {
    {
		CMD_MODE_IF,
        CMD_TYPE_CMD,
		"interface",
		"Config interface",
		config_interface,
		cfg_if_type,
		CMD_FLAG_NEXT
	},
    {
        CMD_MODE_IF,
        CMD_TYPE_CMD,
        "name",
        "Set Interface Name",
        config_if_name,
        cfg_set_str,
        CMD_FLAG_NEXT
    },
    {
        CMD_MODE_IF,
        CMD_TYPE_CMD,
        "enable",
        "Enable Interface",
        config_if_enable,
        NULL,
        CMD_FLAG_CR_ALLOWED
    },
    {
        CMD_MODE_IF,
        CMD_TYPE_CMD,
        "disable",
        "Disable Interface",
        config_if_disable,
        NULL,
        CMD_FLAG_CR_ALLOWED
    },
   {
		CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
		"debug",
		"Set Debug Level",
		NULL,
		cmd_debug,
		CMD_FLAG_NEXT
	},
    {
        CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
        "undebug",
        "Unset Debug Level",
        NULL,
        cmd_undebug,
        CMD_FLAG_NEXT
    },
    {
		CMD_MODE_NONE,
        CMD_TYPE_CMD,
		"show",
		"Show Commands",
		NULL,
		cmd_show,
		CMD_FLAG_NEXT | CMD_FLAG_LAST
	}
};

cdb_node_t cmd_cfg[] = {
	{
		CMD_MODE_CONFIG,
        CMD_TYPE_CMD,
		"interface",
		"Config interface",
		config_interface,
		cfg_if_type,
		CMD_FLAG_NEXT
	},
    {
		CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
		"debug",
		"Set Debug Level",
		NULL,
		cmd_debug,
		CMD_FLAG_NEXT
	},
    {
        CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
        "undebug",
        "Unset Debug Level",
        NULL,
        cmd_undebug,
        CMD_FLAG_NEXT
    },
    {
		CMD_MODE_NONE,
        CMD_TYPE_CMD,
		"show",
		"Show Commands",
		NULL,
		cmd_show,
		CMD_FLAG_NEXT | CMD_FLAG_LAST
	}
};

cdb_node_t cmd_show_if_brief[] = {
    {
        CMD_MODE_NONE,
        CMD_TYPE_CMD,
        "brief",
        "Show Interfase Brief",
        cmd_show_interface_brief,
        NULL,
        CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
    }
};

cdb_node_t cmd_show[] = {
	{
		CMD_MODE_NONE,
        CMD_TYPE_CMD,
		"version",
		"Check Software Version",
		cmd_show_version,
		NULL,
		CMD_FLAG_CR_ALLOWED
	},
    {
        CMD_MODE_NONE,
        CMD_TYPE_CMD,
        "config-veriables",
        "Show Global Config Variables",
        cmd_show_global_config_ver,
        NULL,
        CMD_FLAG_CR_ALLOWED
    },
    {
		CMD_MODE_NONE,
        CMD_TYPE_CMD,
		"debug",
		"Set Debug Level",
		cmd_show_debug,
		NULL,
		CMD_FLAG_CR_ALLOWED
	},
    {
        CMD_MODE_NONE,
        CMD_TYPE_CMD,
        "running-config",
        "Show Running Configurations",
        cmd_show_running_config,
        NULL,
        CMD_FLAG_CR_ALLOWED
    },
    {
        CMD_MODE_NONE,
        CMD_TYPE_CMD,
        "interfaces",
        "Show Interface Configurations",
        cmd_show_interface_config,
        cmd_show_if_brief,
        CMD_FLAG_NEXT | CMD_FLAG_CR_ALLOWED
    },
    {
        CMD_MODE_NONE,
        CMD_TYPE_CMD,
        "cli",
        "Show All Commands",
        cmd_show_cli,
        NULL,
        CMD_FLAG_CR_ALLOWED
    },
	{
		CMD_MODE_NONE,
        CMD_TYPE_CMD,
		"configuration",
		"Show Configurations",
		cmd_show_configurations,
		NULL,
		CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
	}
};

cdb_node_t cmd_set_debug_val[] = {
    {
        CMD_MODE_NONE,
        CMD_TYPE_DECIMAL,
        "",
        "Set Debug Level Value",
        cmd_set_dbg_level_val,
        NULL,
        CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
    },
};

cdb_node_t cmd_set_debug_level[] = {
    {
        CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
        "level",
        "Set Debug Level",
        NULL,
        cmd_set_debug_val,
        CMD_FLAG_NEXT
    },
    {
        CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
        "all",
        "Set all debugging Levels",
        cmd_set_dbg_level_all,
        NULL,
        CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
    },
};

cdb_node_t cmd_debug[] = {
    {
        CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
        "cli",
        "Set cli debugging Levels",
        cmd_set_dbg_cli,
        cmd_set_debug_level,
        CMD_FLAG_LAST | CMD_FLAG_NEXT
    },
};

cdb_node_t cmd_undebug[] = {
    {
        CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
        "cli",
        "Reset all debugging",
        cmd_set_undbg_cli,
        cmd_set_debug_level,
        CMD_FLAG_NEXT
    },
    {
        CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
        "all",
        "Set Debug Level",
        cmd_set_undbg_all,
        NULL,
        CMD_FLAG_LAST | CMD_FLAG_CR_ALLOWED
    },
};

cdb_node_t cmd_enable[] = {
	{
		CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
		"config",
		"Enable Configuration",
		NULL,
		cmd_cfg_terminal,
		CMD_FLAG_NEXT
	},
    {
		CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
		"debug",
		"Set Debug Level",
		NULL,
		cmd_debug,
		CMD_FLAG_NEXT
	},
    {
        CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
        "undebug",
        "Unset Debug Level",
        NULL,
        cmd_undebug,
        CMD_FLAG_NEXT
    },
	{
		CMD_MODE_ENABLE,
        CMD_TYPE_CMD,
		"show",
		"Show Commands",
		NULL,
		cmd_show,
		CMD_FLAG_NEXT | CMD_FLAG_LAST
	}
};

