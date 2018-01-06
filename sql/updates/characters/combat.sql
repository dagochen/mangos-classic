/*
SQLyog Community v12.5.0 (64 bit)
MySQL - 5.7.20-log 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `raidstats_combat` (
	`id` int (11),
	`raidId` int (11),
	`timestamp` datetime ,
	`attackerGUID` int (11),
	`victimGUID` int (11),
	`attackerType` int (11),
	`victimType` int (11),
	`zone` int (11)
); 
