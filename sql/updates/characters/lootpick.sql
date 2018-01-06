/*
SQLyog Community v12.5.0 (64 bit)
MySQL - 5.7.20-log 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `raidstats_lootpick` (
	`id` int (11),
	`raidId` int (11),
	`timestamp` datetime ,
	`looterGUID` int (11),
	`loot` int (11),
	`sourceGUID` int (11),
	`sourceType` int (11),
	`zone` int (11)
); 
