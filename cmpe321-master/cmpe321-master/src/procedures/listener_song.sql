CREATE TABLE `dbtify`.`listener_song` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `listener_id` INT NOT NULL,
  `song_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `listener_idx` (`listener_id` ASC) VISIBLE,
  INDEX `song_idx` (`song_id` ASC) VISIBLE,
    
    FOREIGN KEY (`listener_id`)
    REFERENCES `dbtify`.`listener` (`id`)
    ON DELETE CASCADE,

    FOREIGN KEY (`song_id`)
    REFERENCES `dbtify`.`song` (`id`)
    ON DELETE CASCADE);
