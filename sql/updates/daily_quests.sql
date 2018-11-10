--- daily quest feature
ALTER TABLE `mangos`.`quest_template`   
  ADD COLUMN `Cooldown` INT(11) DEFAULT 0 NOT NULL AFTER `CompleteScript`;

ALTER TABLE `characters`.`character_queststatus`   
  ADD COLUMN `finishTime` BIGINT(20) DEFAULT 0 NOT NULL AFTER `updated`;
