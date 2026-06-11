public class BouquetMonitor {
    // Монитор для синхронизации
    private static final Object monitor = new Object();
    
    // Состояние ресурсов и поставщиков
    private static boolean supplierBusy = false;
    private static int currentSupplier = -1; // 0-V1, 1-V2, 2-V3
    private static boolean[] resourcesReady = new boolean[3]; // готовность ресурсов для составителей
    
    // Поставщик V1 (розы и фиалки)
    static class SupplierV1 extends Thread {
        public void run() {
            try {
                while (true) {
                    synchronized (monitor) {
                        System.out.println("V1: Пытается начать поставку...");
                        
                        // Ждем пока предыдущий поставщик завершит работу
                        while (supplierBusy) {
                            System.out.println("V1: Ждет освобождения монитора...");
                            monitor.wait();
                        }
                        
                        System.out.println("V1: Начал поставку (розы и фиалки)");
                        supplierBusy = true;
                        currentSupplier = 0;
                        resourcesReady[0] = true; // сигнал для S1
                        
                        monitor.notifyAll(); // уведомляем составителей
                        
                        // Ждем пока составитель заберет ресурсы
                        while (supplierBusy && currentSupplier == 0) {
                            System.out.println("V1: Ждет пока составитель заберет ресурсы...");
                            monitor.wait();
                        }
                        
                        System.out.println("V1: Ресурсы потреблены, завершаем поставку");
                    }
                    
                    Thread.sleep(1000);
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
                    synchronized (monitor) {
                        System.out.println("V2: Пытается начать поставку...");
                        
                        while (supplierBusy) {
                            System.out.println("V2: Ждет освобождения монитора...");
                            monitor.wait();
                        }
                        
                        System.out.println("V2: Начал поставку (пионы и розы)");
                        supplierBusy = true;
                        currentSupplier = 1;
                        resourcesReady[1] = true;
                        
                        monitor.notifyAll();
                        
                        while (supplierBusy && currentSupplier == 1) {
                            System.out.println("V2: Ждет пока составитель заберет ресурсы...");
                            monitor.wait();
                        }
                        
                        System.out.println("V2: Ресурсы потреблены, завершаем поставку");
                    }
                    
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
                    synchronized (monitor) {
                        System.out.println("V3: Пытается начать поставку...");
                        
                        while (supplierBusy) {
                            System.out.println("V3: Ждет освобождения монитора...");
                            monitor.wait();
                        }
                        
                        System.out.println("V3: Начал поставку (пионы и фиалки)");
                        supplierBusy = true;
                        currentSupplier = 2;
                        resourcesReady[2] = true;
                        
                        monitor.notifyAll();
                        
                        while (supplierBusy && currentSupplier == 2) {
                            System.out.println("V3: Ждет пока составитель заберет ресурсы...");
                            monitor.wait();
                        }
                        
                        System.out.println("V3: Ресурсы потреблены, завершаем поставку");
                    }
                    
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
                    synchronized (monitor) {
                        System.out.println("S1: Проверяет доступность ресурсов...");
                        
                        // Ждем сигнала от V1
                        while (!resourcesReady[0]) {
                            System.out.println("S1: Ждет ресурсы от V1...");
                            monitor.wait();
                        }
                        
                        System.out.println("S1: Получил ресурсы от V1");
                        System.out.println("S1: Составляет букет (розы + фиалки + свои пионы)");
                        
                        // Забираем ресурсы
                        resourcesReady[0] = false;
                        supplierBusy = false;
                        currentSupplier = -1;
                        
                        System.out.println("S1: Букет готов, освобождает поставщика");
                        monitor.notifyAll(); // уведомляем поставщиков
                    }
                    
                    Thread.sleep(800); // время на составление букета
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
                    synchronized (monitor) {
                        System.out.println("S2: Проверяет доступность ресурсов...");
                        
                        while (!resourcesReady[1]) {
                            System.out.println("S2: Ждет ресурсы от V2...");
                            monitor.wait();
                        }
                        
                        System.out.println("S2: Получил ресурсы от V2");
                        System.out.println("S2: Составляет букет (розы + пионы + свои фиалки)");
                        
                        resourcesReady[1] = false;
                        supplierBusy = false;
                        currentSupplier = -1;
                        
                        System.out.println("S2: Букет готов, освобождает поставщика");
                        monitor.notifyAll();
                    }
                    
                    Thread.sleep(800);
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
                    synchronized (monitor) {
                        System.out.println("S3: Проверяет доступность ресурсов...");
                        
                        while (!resourcesReady[2]) {
                            System.out.println("S3: Ждет ресурсы от V3...");
                            monitor.wait();
                        }
                        
                        System.out.println("S3: Получил ресурсы от V3");
                        System.out.println("S3: Составляет букет (фиалки + пионы + свои розы)");
                        
                        resourcesReady[2] = false;
                        supplierBusy = false;
                        currentSupplier = -1;
                        
                        System.out.println("S3: Букет готов, освобождает поставщика");
                        monitor.notifyAll();
                    }
                    
                    Thread.sleep(800);
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== ЗАПУСК ПРОГРАММЫ С МОНИТОРАМИ ===");
        
        // Инициализация состояний
        for (int i = 0; i < 3; i++) {
            resourcesReady[i] = false;
        }
        
        // Создаем и запускаем потоки
        new SupplierV1().start();
        new SupplierV2().start();
        new SupplierV3().start();
        new ComposerS1().start();
        new ComposerS2().start();
        new ComposerS3().start();
    }
}
