CREATE TABLE `dbtify`.`listener_album` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `listener_id` INT NOT NULL,
  `album_id` INT NOT NULL,
  PRIMARY KEY (`id`),
  INDEX `listener_idx` (`listener_id` ASC) VISIBLE,
  INDEX `album_idx` (`album_id` ASC) VISIBLE,
    
    FOREIGN KEY (`listener_id`)
    REFERENCES `dbtify`.`listener` (`id`)
    ON DELETE CASCADE,

    FOREIGN KEY (`album_id`)
    REFERENCES `dbtify`.`album` (`id`)
    ON DELETE CASCADE
);
