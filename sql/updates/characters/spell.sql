/*
SQLyog Community v12.5.0 (64 bit)
MySQL - 5.7.20-log 
*********************************************************************
*/
/*!40101 SET NAMES utf8 */;

create table `raidstats_spell` (
	`id` int (11),
	`timestamp` datetime ,
	`casterGUID` int (11),
	`targetGUID` int (11),
	`raidId` int (11),
	`spellId` int (11),
	`damage` int (11),
	`absorb` int (11),
	`resist` int (11),
	`blocked` int (11),
	`casterType` tinyint (1),
	`targetType` tinyint (1),
	`hitresult` int (11),
	`missInfo` int (11),
	`resistance` int (11),
	`school` int (11),
	`isOverTime` tinyint (1),
	`zone` int (11)
); 
