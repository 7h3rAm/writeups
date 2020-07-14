<?php
/**
 * The base configuration for WordPress
 *
 * The wp-config.php creation script uses this file during the
 * installation. You don't have to use the web site, you can
 * copy this file to "wp-config.php" and fill in the values.
 *
 * This file contains the following configurations:
 *
 * * MySQL settings
 * * Secret keys
 * * Database table prefix
 * * ABSPATH
 *
 * @link https://codex.wordpress.org/Editing_wp-config.php
 *
 * @package WordPress
 */

// ** MySQL settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define('DB_NAME', 'wordpress');

/** MySQL database username */
define('DB_USER', 'Admin');

/** MySQL database password */
define('DB_PASSWORD', 'TogieMYSQL12345^^');

/** MySQL hostname */
define('DB_HOST', 'localhost');

/** Database Charset to use in creating database tables. */
define('DB_CHARSET', 'utf8');

/** The Database Collate type. Don't change this if in doubt. */
define('DB_COLLATE', '');

/**#@+
 * Authentication Unique Keys and Salts.
 *
 * Change these to different unique phrases!
 * You can generate these using the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}
 * You can change these at any point in time to invalidate all existing cookies. This will force all users to have to log in again.
 *
 * @since 2.6.0
 */

define('AUTH_KEY',         'SAq-)W,-K9tFcW(=?ro4SJ5)R.mx%+@KL-I@PB{<-i>g3n^1|E<-uN|}F;:PbMYJ');
define('SECURE_AUTH_KEY',  'u .o%Ld%m27waNqK+*`~&j6~v!d7vI|OwA|hd8%r#ri_`WRIcCN-KiTSWmk)1;xG');
define('LOGGED_IN_KEY',    'iX^NN~N7R5Mdmeh:$iLY60r~K[)^f5vk`wGDO30r8Ns)gA17FRt2|$#S!Lq@-<|`');
define('NONCE_KEY',        ',_xAk=+)B7f_a|#J44}qWca!=`s4{C2.Xe>sY%4Ybd5*3z9WRH-ysm=.|Gm^McvU');
define('AUTH_SALT',        '(:^<BWwzWYx ,f^9anxD,+V+2-&,VJ@@)U7CSzjv_MvD67>?05ihCG]Q1K:_7Xsa');
define('SECURE_AUTH_SALT', 'ud]}}0rWRMGZ+a`Hky G7|i|+c7YyH4=l#5{/1R=|]PYrOmN{&0JuqkO=o5vyGg5');
define('LOGGED_IN_SALT',   '=M_DRp%vGmijIhl%K!(v>:,*RR<cl9ahav%{q`&I/0HD/$W/LK:mxR37PKh?Zzi8');
define('NONCE_SALT',       'ABOgE>G:U;Q/hO^>jBG5e96OL6+{=mV,|2S~c,~dhVa!E/&Q[Mc8#IgVTuXAI}sY');

;

/**#@-*/

/**
 * WordPress Database Table prefix.
 *
 * You can have multiple installations in one database if you give each
 * a unique prefix. Only numbers, letters, and underscores please!
 */
$table_prefix  = 'wp_';

/**
 * For developers: WordPress debugging mode.
 *
 * Change this to true to enable the display of notices during development.
 * It is strongly recommended that plugin and theme developers use WP_DEBUG
 * in their development environments.
 *
 * For information on other constants that can be used for debugging,
 * visit the Codex.
 *
 * @link https://codex.wordpress.org/Debugging_in_WordPress
 */
define('WP_DEBUG', false);

/* That's all, stop editing! Happy blogging. */

/** Absolute path to the WordPress directory. */
if ( !defined('ABSPATH') )
	define('ABSPATH', dirname(__FILE__) . '/');

/* Dynamic site URL added by Togie */
$currenthost = "http://".$_SERVER['HTTP_HOST'];
$currentpath = preg_replace('@/+$@','',dirname($_SERVER['SCRIPT_NAME']));
$currentpath = preg_replace('/\/wp.+/','',$currentpath);
define('WP_HOME',$currenthost.$currentpath);
define('WP_SITEURL',$currenthost.$currentpath);
define('WP_CONTENT_URL', $currenthost.$currentpath.'/wp-content');
define('WP_PLUGIN_URL', $currenthost.$currentpath.'/wp-content/plugins');
define('DOMAIN_CURRENT_SITE', $currenthost.$currentpath );
@define('ADMIN_COOKIE_PATH', './');


/** Sets up WordPress vars and included files. */
require_once(ABSPATH . 'wp-settings.php');
