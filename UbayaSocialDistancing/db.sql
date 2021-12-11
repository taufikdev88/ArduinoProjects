-- --------------------------------------------------------
-- Host:                         127.0.0.1
-- Server version:               10.4.17-MariaDB - mariadb.org binary distribution
-- Server OS:                    Win64
-- HeidiSQL Version:             11.3.0.6295
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


-- Dumping database structure for ubayasocialdistancing
DROP DATABASE IF EXISTS `ubayasocialdistancing`;
CREATE DATABASE IF NOT EXISTS `ubayasocialdistancing` /*!40100 DEFAULT CHARACTER SET utf8mb4 */;
USE `ubayasocialdistancing`;

-- Dumping structure for table ubayasocialdistancing.tb_device
DROP TABLE IF EXISTS `tb_device`;
CREATE TABLE IF NOT EXISTS `tb_device` (
  `mac` varchar(20) NOT NULL,
  `username` varchar(30) DEFAULT 'none',
  `detail` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`mac`),
  KEY `FK__tb_user` (`username`),
  CONSTRAINT `FK__tb_user` FOREIGN KEY (`username`) REFERENCES `tb_user` (`username`) ON DELETE SET NULL ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Data exporting was unselected.

-- Dumping structure for table ubayasocialdistancing.tb_log
DROP TABLE IF EXISTS `tb_log`;
CREATE TABLE IF NOT EXISTS `tb_log` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `mac1` varchar(22) DEFAULT NULL,
  `mac2` varchar(22) DEFAULT NULL,
  `username1` varchar(30) DEFAULT NULL,
  `username2` varchar(30) DEFAULT NULL,
  `rssi` int(11) DEFAULT NULL,
  `created_at` timestamp NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `FK_tb_log_tb_user` (`username1`),
  KEY `FK_tb_log_tb_user_2` (`username2`),
  KEY `FK_tb_log_tb_device` (`mac1`) USING BTREE,
  KEY `FK_tb_log_tb_device_2` (`mac2`),
  CONSTRAINT `FK_tb_log_tb_device` FOREIGN KEY (`mac1`) REFERENCES `tb_device` (`mac`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_log_tb_device_2` FOREIGN KEY (`mac2`) REFERENCES `tb_device` (`mac`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_log_tb_user` FOREIGN KEY (`username1`) REFERENCES `tb_user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `FK_tb_log_tb_user_2` FOREIGN KEY (`username2`) REFERENCES `tb_user` (`username`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=79 DEFAULT CHARSET=utf8mb4;

-- Data exporting was unselected.

-- Dumping structure for table ubayasocialdistancing.tb_master
DROP TABLE IF EXISTS `tb_master`;
CREATE TABLE IF NOT EXISTS `tb_master` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `rssi1` int(11) NOT NULL DEFAULT 0,
  `rssi2` int(11) NOT NULL DEFAULT 0,
  `rssi3` int(11) NOT NULL DEFAULT 0,
  `rssi4` int(11) NOT NULL DEFAULT 0,
  `created_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=utf8mb4;

-- Data exporting was unselected.

-- Dumping structure for table ubayasocialdistancing.tb_user
DROP TABLE IF EXISTS `tb_user`;
CREATE TABLE IF NOT EXISTS `tb_user` (
  `username` varchar(30) NOT NULL,
  `password` varchar(30) NOT NULL,
  `nama` varchar(50) NOT NULL,
  `role` enum('U','A') DEFAULT 'U',
  `registered_at` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- Data exporting was unselected.

/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
