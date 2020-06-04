-- Adminer 4.6.3 MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

SET NAMES utf8mb4;

DROP DATABASE IF EXISTS `kiosk`;
CREATE DATABASE `kiosk` /*!40100 DEFAULT CHARACTER SET utf8mb4 */;
USE `kiosk`;

DROP TABLE IF EXISTS `categories`;
CREATE TABLE `categories` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  `code` varchar(50) NOT NULL,
  `discount` int(11) NOT NULL DEFAULT '0',
  `is_discount` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO `categories` (`id`, `name`, `code`, `discount`, `is_discount`) VALUES
(1,	'Skin Care Products',	'SK',	15,	1),
(2,	'Make-up Products',	'MU',	30,	0);

DROP TABLE IF EXISTS `items`;
CREATE TABLE `items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `code` varchar(50) NOT NULL,
  `name` varchar(50) NOT NULL,
  `stock` int(11) NOT NULL DEFAULT '0',
  `price` float(10,2) NOT NULL,
  `category` int(11) NOT NULL,
  `is_listed` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  UNIQUE KEY `code` (`code`),
  KEY `category` (`category`),
  CONSTRAINT `items_ibfk_1` FOREIGN KEY (`category`) REFERENCES `categories` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO `items` (`id`, `code`, `name`, `stock`, `price`, `category`, `is_listed`) VALUES
(1,	'SK1',	'CLEANSER',	44,	13.00,	1,	1),
(2,	'SK2',	'TONER',	99,	15.50,	1,	0),
(3,	'SK3',	'ANTIOXIDANT SERUM',	63,	19.50,	1,	1),
(4,	'MU1',	'FOUNDATION',	80,	15.50,	2,	1),
(5,	'MU2',	'BLUSH',	56,	13.30,	2,	1),
(6,	'MU3',	'CONCEALER',	81,	21.40,	2,	1),
(7,	'MU4',	'BB CREAM',	28,	14.40,	2,	1),
(8,	'SK4',	'EYE CREAM',	32,	15.50,	1,	1);

DROP TABLE IF EXISTS `orders`;
CREATE TABLE `orders` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `total_price` float(10,2) NOT NULL,
  `after_discount` float(10,2) NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


DROP TABLE IF EXISTS `order_items`;
CREATE TABLE `order_items` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `orders` int(11) NOT NULL,
  `items` int(11) NOT NULL,
  `amount` int(11) NOT NULL,
  `total_price` float(10,2) NOT NULL,
  `after_discount` float(10,2) NOT NULL,
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `orders` (`orders`),
  KEY `items` (`items`),
  CONSTRAINT `order_items_ibfk_1` FOREIGN KEY (`orders`) REFERENCES `orders` (`id`),
  CONSTRAINT `order_items_ibfk_2` FOREIGN KEY (`items`) REFERENCES `items` (`id`),
  CONSTRAINT `order_items_ibfk_3` FOREIGN KEY (`orders`) REFERENCES `orders` (`id`),
  CONSTRAINT `order_items_ibfk_4` FOREIGN KEY (`items`) REFERENCES `items` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;


DROP TABLE IF EXISTS `staff`;
CREATE TABLE `staff` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(50) NOT NULL,
  `password` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  `is_admin` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO `staff` (`id`, `username`, `password`, `name`, `is_admin`) VALUES
(1,	'admin',	'03ac674216f3e15c761ee1a5e255f067953623c8b388b4459e13f978d7c846f4',	'Admin',	1);

-- 2020-06-04 10:52:36
