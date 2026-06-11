import java.util.concurrent.Semaphore;

public class BouquetSemaphore {
    // Семафоры для поставщиков
    private static Semaphore supplierMutex = new Semaphore(1); // взаимное исключение для поставщиков
    private static Semaphore[] supplierReady = new Semaphore[3]; // поставщик готов
    private static Semaphore[] composerTaken = new Semaphore[3]; // составитель взял ресурсы
    
    // Ресурсы (true - доступны)
    private static boolean[] resourcesAvailable = new boolean[3]; // 0-розы, 1-фиалки, 2-пионы
    
    static {
        for (int i = 0; i < 3; i++) {
            supplierReady[i] = new Semaphore(0);
            composerTaken[i] = new Semaphore(0);
            resourcesAvailable[i] = false;
        }
    }
    
    // Поставщик V1 (розы и фиалки)
    static class SupplierV1 extends Thread {
        public void run() {
            try {
                while (true) {
                    System.out.println("V1: Пытается начать поставку...");
                    supplierMutex.acquire();
                    System.out.println("V1: Начал поставку (розы и фиалки)");
                    
                    // Поставляем ресурсы
                    resourcesAvailable[0] = true; // розы
                    resourcesAvailable[1] = true; // фиалки
                    
                    System.out.println("V1: Ресурсы поставлены, ждем составителя...");
                    supplierReady[0].release(); // сигнал для S1
                    composerTaken[0].acquire(); // ждем пока S1 заберет
                    
                    System.out.println("V1: Ресурсы потреблены, завершаем поставку");
                    supplierMutex.release();
                    
                    Thread.sleep(1000); // пауза между поставками
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    
    // Поставщик V2 (пионы и розы)
    static class SupplierV2 extends Thread {
        public void run() {
            try {
                while (true) {
                    System.out.println("V2: Пытается начать поставку...");
                    supplierMutex.acquire();
                    System.out.println("V2: Начал поставку (пионы и розы)");
                    
                    resourcesAvailable[2] = true; // пионы
                    resourcesAvailable[0] = true; // розы
                    
                    System.out.println("V2: Ресурсы поставлены, ждем составителя...");
                    supplierReady[1].release();
                    composerTaken[1].acquire();
                    
                    System.out.println("V2: Ресурсы потреблены, завершаем поставку");
                    supplierMutex.release();
                    
                    Thread.sleep(1000);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    
    // Поставщик V3 (пионы и фиалки)
    static class SupplierV3 extends Thread {
        public void run() {
            try {
                while (true) {
                    System.out.println("V3: Пытается начать поставку...");
                    supplierMutex.acquire();
                    System.out.println("V3: Начал поставку (пионы и фиалки)");
                    
                    resourcesAvailable[2] = true; // пионы
                    resourcesAvailable[1] = true; // фиалки
                    
                    System.out.println("V3: Ресурсы поставлены, ждем составителя...");
                    supplierReady[2].release();
                    composerTaken[2].acquire();
                    
                    System.out.println("V3: Ресурсы потреблены, завершаем поставку");
                    supplierMutex.release();
                    
                    Thread.sleep(1000);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    
    // Составитель S1 (имеет пионы, нужны розы и фиалки)
    static class ComposerS1 extends Thread {
        public void run() {
            try {
                while (true) {
                    System.out.println("S1: Ждет ресурсы от V1...");
                    supplierReady[0].acquire();
                    System.out.println("S1: Получил ресурсы от V1");
                    
                    // Проверяем что нужные ресурсы доступны
                    if (resourcesAvailable[0] && resourcesAvailable[1]) {
                        System.out.println("S1: Составляет букет (розы + фиалки + свои пионы)");
                        Thread.sleep(500);
                        
                        // Забираем ресурсы
                        resourcesAvailable[0] = false;
                        resourcesAvailable[1] = false;
                        
                        System.out.println("S1: Букет готов, освобождаем поставщика");
                        composerTaken[0].release();
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    
    // Составитель S2 (имеет фиалки, нужны розы и пионы)
    static class ComposerS2 extends Thread {
        public void run() {
            try {
                while (true) {
                    System.out.println("S2: Ждет ресурсы от V2...");
                    supplierReady[1].acquire();
                    System.out.println("S2: Получил ресурсы от V2");
                    
                    if (resourcesAvailable[0] && resourcesAvailable[2]) {
                        System.out.println("S2: Составляет букет (розы + пионы + свои фиалки)");
                        Thread.sleep(500);
                        
                        resourcesAvailable[0] = false;
                        resourcesAvailable[2] = false;
                        
                        System.out.println("S2: Букет готов, освобождаем поставщика");
                        composerTaken[1].release();
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    
    // Составитель S3 (имеет розы, нужны фиалки и пионы)
    static class ComposerS3 extends Thread {
        public void run() {
            try {
                while (true) {
                    System.out.println("S3: Ждет ресурсы от V3...");
                    supplierReady[2].acquire();
                    System.out.println("S3: Получил ресурсы от V3");
                    
                    if (resourcesAvailable[1] && resourcesAvailable[2]) {
                        System.out.println("S3: Составляет букет (фиалки + пионы + свои розы)");
                        Thread.sleep(500);
                        
                        resourcesAvailable[1] = false;
                        resourcesAvailable[2] = false;
                        
                        System.out.println("S3: Букет готов, освобождаем поставщика");
                        composerTaken[2].release();
                    }
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== ЗАПУСК ПРОГРАММЫ С СЕМАФОРАМИ ===");
        
        // Создаем и запускаем потоки
        new SupplierV1().start();
        new SupplierV2().start();
        new SupplierV3().start();
        new ComposerS1().start();
        new ComposerS2().start();
        new ComposerS3().start();
    }
}