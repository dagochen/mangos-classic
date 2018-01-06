/*
SQLyog Community v12.5.0 (64 bit)
MySQL - 5.7.20-log 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `raidstats_heal` (
	`id` int (11),
	`raidId` int (11),
	`healerGUID` int (11),
	`targetGUID` int (11),
	`spellId` int (11),
	`healerType` tinyint (1),
	`targetType` tinyint (1),
	`rawHeal` int (11),
	`overHeal` int (11),
	`efficientHeal` int (11),
	`timestamp` datetime ,
	`isOverTime` tinyint (1),
	`zone` int (11)
); 
