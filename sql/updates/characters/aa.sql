/*
SQLyog Community v12.5.0 (64 bit)
MySQL - 5.7.20-log 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `raidstats_autoattack` (
	`id` int (11),
	`timestamp` datetime ,
	`raidId` int (11),
	`attackerGUID` int (11),
	`targetGUID` int (11),
	`attackType` int (11),
	`damage` int (11),
	`hitoutcome` int (11),
	`attackerType` int (11),
	`targetType` int (11),
	`zone` int (11)
); 
