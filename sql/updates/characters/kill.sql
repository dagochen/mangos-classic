/*
SQLyog Community v12.5.0 (64 bit)
MySQL - 5.7.20-log 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `raidstats_kill` (
	`id` int (11),
	`raidId` int (11),
	`timestamp` datetime ,
	`killerGUID` int (11),
	`victimGUID` int (11),
	`killerType` tinyint (1),
	`victimType` tinyint (1),
	`zone` int (11)
); 
