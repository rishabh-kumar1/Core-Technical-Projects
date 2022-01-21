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
define( 'DB_NAME', 'local' );

/** MySQL database username */
define( 'DB_USER', 'root' );

/** MySQL database password */
define( 'DB_PASSWORD', 'root' );

/** MySQL hostname */
define( 'DB_HOST', 'localhost' );

/** Database Charset to use in creating database tables. */
define( 'DB_CHARSET', 'utf8' );

/** The Database Collate type. Don't change this if in doubt. */
define( 'DB_COLLATE', '' );

/**
 * Authentication Unique Keys and Salts.
 *
 * Change these to different unique phrases!
 * You can generate these using the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}
 * You can change these at any point in time to invalidate all existing cookies. This will force all users to have to log in again.
 *
 * @since 2.6.0
 */
define('AUTH_KEY',         'nB+zRTfhzrmNAkJkXdV98bHg7noDrh30zUAN5VNqgruN+At49sAQifippdRJvqd1b65aoMqB+D5/uIN7FlkaBQ==');
define('SECURE_AUTH_KEY',  'yWJkKfr41Q+xGMqRhaiyKKfEkji8Mfs0s2ZEHygcviTBCvow853+c/nNvKdAr9yywMb3me9ThWPLUdIBDKkPKw==');
define('LOGGED_IN_KEY',    'RC0dBXnZa5oaMYQ2HFI9NNCJWznC2KXkRZhXoxXcL0efpKBPvRiL0GCiVinOiU5LCQLZ0VGUXEk8gULfOCVMJA==');
define('NONCE_KEY',        'xDNh2Fr2GDylBD378w4HDghlpvlK478svLbW9kIU0tm1sey1l59iotgJNrJqKkv9oXW6z52CLdz5x+l3cHUe7A==');
define('AUTH_SALT',        'BUhVn2XMBoD7QLEE0ekR/plGteehDndjYif+USr8xnX66wucsr9Gl1BJiuzW3j51Ik9W7UPNfzeBj0uaLa07zA==');
define('SECURE_AUTH_SALT', 'buZAAFF+US4zSHAHbr4ec0bAmOkIpzkvcoKUwphe7l6RPvFv1sH5yCoLyFwyRec5yQOiAXk4uJUsvic2mEdkGw==');
define('LOGGED_IN_SALT',   'ri/gquxEXn7pwszPkX0X/oqtDrzK2WMMIOZDCpJtItoJkDINDznsR1gaZT7JM5y24X94QNUFgJtv3Ieu9dSS0A==');
define('NONCE_SALT',       '8NoXAB8HAWGr6TzPCUJXko4aPy55foPyNZIPVPBw6Mgf2JosgGbC8jD62hXhybliYRpCbYUoOmrXDe0ZqzVfhA==');

/**
 * WordPress Database Table prefix.
 *
 * You can have multiple installations in one database if you give each
 * a unique prefix. Only numbers, letters, and underscores please!
 */
$table_prefix = 'wp_';




/* That's all, stop editing! Happy publishing. */

/** Absolute path to the WordPress directory. */
if ( ! defined( 'ABSPATH' ) ) {
	define( 'ABSPATH', dirname( __FILE__ ) . '/' );
}

/** Sets up WordPress vars and included files. */
require_once ABSPATH . 'wp-settings.php';
